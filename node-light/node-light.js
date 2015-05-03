var dgram = require('dgram');
var fs    = require('fs');
var nconf = require('nconf');
var express = require('express');
var app = express();

require('mkdirp').mkdirp(__dirname + '/log')

nconf.file({file: require('path').resolve(__dirname, 'storage.json') });
var light_state = nconf.get('light_state');
var power_state = nconf.get('power_state');


var winston = require('winston');

var logger = new (winston.Logger)({
	transports: [
		new (winston.transports.Console)({colorize: 'all'}),
		new winston.transports.DailyRotateFile({
			filename: __dirname + '/log/logfile.log',
			datePattern: '.dd-MM-yyyy'
		})
	]
});


app.use(require('body-parser').json());

app.all('*', function(req, res, next) {
	res.set({'Content-Type': 'application/json; charset=utf-8', 'cache-control': 'max-age=0, no-cache, no-store, must-revalidate'});
	res.set({'Access-Control-Allow-Origin': '*'});
	logger.info(req.method + ' ' + req.url);
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
	logger.info('state='+ req.body.state);
    sendUDP(req.params[0],req.body.state);
});

app.get(/^\/power\/([1-5])$/, function(req, res) {
	res.send(JSON.stringify(power_state[(req.params[0])]));
});
app.put(/^\/power\/([1-5])$/, function(req, res) {
    res.end();
	logger.info('state='+ req.body.state);
    sendUDPPower(req.params[0],req.body.state);
});

app.options('*', function(req, res) {
	res.set({'Access-Control-Allow-Headers': 'access-control-allow-origin, Content-Type, X-Requested-With'});
	res.set({'Access-Control-Allow-Methods': 'GET,PUT'});
	res.send(200);
});

app.all('*', function(req, res) {
	res.send(405, 'Method not allowed');
	logger.info('/: 405');
});

app.listen(8080);
logger.info('Server started on port 8080');



function exitHandler(options, err) {
	logger.info('\n Start exithandler()');
	nconf.set('light_state',light_state);
	nconf.set('power_state',power_state);
	nconf.save();
	logger.info('successfull Good Bye');
    if (options.exit) process.exit();
}
//catches ctrl+c event
process.on('SIGINT', exitHandler.bind(null, {exit:true}));
//catches sigterm event
process.on('SIGTERM', exitHandler.bind(null, {exit:true}));
//catches uncaught exceptions
process.on('uncaughtException', exitHandler.bind(null, {exit:true}));



var socket = dgram.createSocket('udp4');

socket.on('message', function (data) {
	receiveUDP(data);
});

socket.on("listening", function () {
  var address = socket.address();
  logger.info("server listening " + address.address + ":" + address.port);
});

socket.bind(2342, function() {
  logger.info('server listing on 2342');
});


receiveUDP = function (data) {
        logger.info('receiveUDP Data...');

	light_lut = new Array(1,2,3,4,6,7,8,5);
	light_lut_state = new Array('off','on');

	if (data.length == 2 && data[0] <= light_lut.length && data[1] <= 1) {
		light_state[(light_lut[(data[0])])].state = light_lut_state[(data[1])];
		logger.info('receiveUDP: ' + data[0] + ' => ' + light_lut[(data[0])] + ' ' + data[1] + ' => ' + light_lut_state[(data[1])] );
	}

	power_lut = new Array(120,140,141,142,143);
	power_lut_state = new Array('off','on');

	if (data.length == 2 && data[0] == 10 && data[1] <= 1) {
		power_state[1].state = power_lut_state[(data[1])];
		logger.info('receiveUDP: ' + data[0] + ' => ' + '120' + ' ' + data[1] + ' => ' + power_lut_state[(data[1])] );
	}
	if (data.length == 2 && data[0] == 140 && data[1] <= 1) {
		power_state[2].state = power_lut_state[(data[1])];
		logger.info('receiveUDP: ' + data[0] + ' => ' + '140' + ' ' + data[1] + ' => ' + power_lut_state[(data[1])] );
	}
	if (data.length == 2 && data[0] == 141 && data[1] <= 1) {
		power_state[3].state = power_lut_state[(data[1])];
		logger.info('receiveUDP: ' + data[0] + ' => ' + '141' + ' ' + data[1] + ' => ' + power_lut_state[(data[1])] );
	}
	if (data.length == 2 && data[0] == 142 && data[1] <= 1) {
		power_state[4].state = power_lut_state[(data[1])];
		logger.info('receiveUDP: ' + data[0] + ' => ' + '142' + ' ' + data[1] + ' => ' + power_lut_state[(data[1])] );
	}
	if (data.length == 2 && data[0] == 143 && data[1] <= 1) {
		power_state[5].state = power_lut_state[(data[1])];
		logger.info('receiveUDP: ' + data[0] + ' => ' + '143' + ' ' + data[1] + ' => ' + power_lut_state[(data[1])] );
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
		client.send(packet, 0, 4, 1337, 'licht.shack', function(err, bytes) { logger.info(err) });
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

	logger.info('sendUDP: ' + state);
	if (typeof(state) == 'undefined') {}	
	else if (state == 'on')  { state_old = 1; }
	else if (state == 'off') { state_old = 0; }

    if (state_old == 1) { power_state[id].state = 'on' }
    if (state_old == 0) { power_state[id].state = 'off'}

	if (state_old != -1) {
		var packet = new Buffer([0xA5,0x5A,id_eltako,state_old]);
		client.send(packet, 0, 4, 1337, 'licht.shack', function(err, bytes) { logger.info(err) });
	}
}


