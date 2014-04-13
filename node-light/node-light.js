var dgram = require('dgram');
var fs    = require('fs');
var nconf = require('nconf');
var express = require('express');
var app = express();

nconf.file({file: require('path').resolve(__dirname, 'storage.json') });
var light_state = nconf.get('light_state');
var power_state = nconf.get('power_state');


app.use(require('body-parser').json());

app.all('*', function(req, res, next) {
	res.set({'Content-Type': 'application/json; charset=utf-8', 'cache-control': 'max-age=0, no-cache, no-store, must-revalidate'});
	console.log(req.method + ' ' + req.url);
	next();
});

app.all('/power/haupt'     , function(req, res) {	res.redirect(307,'1'); });
app.all('/power/dusche'    , function(req, res) {	res.redirect(307,'2'); });
app.all('/power/warmwasser', function(req, res) {	res.redirect(307,'3'); });
app.all('/power/or'        , function(req, res) {	res.redirect(307,'4'); });
app.all('/power/kueche'    , function(req, res) {	res.redirect(307,'5'); });

app.get(/^\/lounge\/([1-8])$/, function(req, res) {
	res.send(JSON.stringify(light_state[(req.params[0])]));
});
app.put(/^\/lounge\/([1-8])$/, function(req, res) {
    res.end();
	console.log('state='+ req.body.state);
    sendUDP(req.params[0],req.body.state);
});

app.get(/^\/power\/([1-5])$/, function(req, res) {
	res.send(JSON.stringify(power_state[(req.params[0])]));
});
app.put(/^\/power\/([1-5])$/, function(req, res) {
    res.end();
	console.log('state='+ req.body.state);
    sendUDPPower(req.params[0],req.body.state);
});

app.all('*', function(req, res) {
	res.send(405, 'Method not allowed');
	console.log('/: 405');
});

app.listen(8080);
console.log('Server started on port 8080');



function exitHandler(options, err) {
	console.log('\n Start exithandler()');
	nconf.set('light_state',light_state);
	nconf.set('power_state',power_state);
	nconf.save();
	console.log('successfull Good Bye');
    if (options.exit) process.exit();
}
//catches ctrl+c event
process.on('SIGINT', exitHandler.bind(null, {exit:true}));
//catches uncaught exceptions
process.on('uncaughtException', exitHandler.bind(null, {exit:true}));



var socket = dgram.createSocket('udp4');

socket.on('message', function (data) {
	receiveUDP(data);
});

socket.on("listening", function () {
  var address = socket.address();
  console.log("server listening " + address.address + ":" + address.port);
});

socket.bind(2342, function() {
  console.log('server listing on 2342');
});


receiveUDP = function (data) {
	light_lut = new Array(1,2,3,4,6,7,8,5);
	light_lut_state = new Array('off','on');

	if (data.length == 2 && data[0] <= light_lut.length && data[1] <= 1) {
		light_state[(light_lut[(data[0])])].state = light_lut_state[(data[1])];
		console.log('receiveUDP: ' + data[0] + ' => ' + light_lut[(data[0])] + ' ' + data[1] + ' => ' + light_lut_state[(data[1])] );
	}
}


var client = dgram.createSocket('udp4');

sendUDP = function (id, state) {
	var state_old = -1;
	var id_old = 0;
	if (id == 1) id_old = 0;
	else if (id == 2) id_old = 1;
	else if (id == 3) id_old = 2;
	else if (id == 4) id_old = 3;
	else if (id == 5) id_old = 7;
	else if (id == 6) id_old = 4;
	else if (id == 7) id_old = 5;
	else if (id == 8) id_old = 6;

	if (typeof(state) == 'undefined') {}	
	else if (state == 'on') {  state_old = 1; }
	else if (state == 'off') { state_old = 0; }

	if (state_old != -1) {
		var packet = new Buffer([0xA5,0x5A,id_old,state_old]);
		client.send(packet, 0, 4, 1337, 'licht.shack', function(err, bytes) { console.log(err) });
	}
}

sendUDPPower = function (id, state) {
	var state_old = 0;
	var id_eltako = 0
	if (id == 1) id_eltako = 120;
	if (id == 2) id_eltako = 140;
	if (id == 3) id_eltako = 141;
	if (id == 4) id_eltako = 142;
	if (id == 5) id_eltako = 143;

	console.log('sendUDP: ' + state);
	if (typeof(state) == 'undefined') {}	
	else if (state == 'on')  { state_old = 1; }
	else if (state == 'off') { state_old = 0; }

    if (state_old == 1) { power_state[id].state = 'on' }
    if (state_old == 0) { power_state[id].state = 'off'}

	if (state_old != -1) {
		var packet = new Buffer([0xA5,0x5A,id_eltako,state_old]);
		client.send(packet, 0, 4, 1337, 'licht.shack', function(err, bytes) { console.log(err) });
	}
}


