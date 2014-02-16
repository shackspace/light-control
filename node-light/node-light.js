var http  = require('http');
var url   = require('url');
var dgram = require('dgram');
var fs    = require('fs');

var light_state = [
{},
{"id":1,"state":"notdef"},
{"id":2,"state":"notdef"},
{"id":3,"state":"notdef"},
{"id":4,"state":"notdef"},
{"id":5,"state":"notdef"},
{"id":6,"state":"notdef"},
{"id":7,"state":"notdef"},
{"id":8,"state":"notdef"}
]

var power_state = [
{},
{"id":1,"state":"notdef"},
{"id":2,"state":"notdef"},
{"id":3,"state":"notdef"},
{"id":4,"state":"notdef"},
]





http.createServer(function (req, res) {
  var urlObj = url.parse(req.url);
  var path = urlObj.pathname.substr(1).split('/');
  
  switch (req.method) {
    case 'GET':
     if (path.length == 2) {
       if (path[0] == "lounge" && path[1] >= 1 && path[1] <= 8) {
         res.writeHead(200, {'Content-Type': 'application/json; charset=utf-8', 'cache-control': 'max-age=0, no-cache, no-store, must-revalidate'});
         console.log('GET-methode');
         console.log(path[0]);
         console.log(path[1]);
         res.end(JSON.stringify(light_state[(path[1])]));
       } else if (path[0] == "power" && path[1] >= 1 && path[1] <= 4) {
         res.writeHead(200, {'Content-Type': 'application/json; charset=utf-8', 'cache-control': 'max-age=0, no-cache, no-store, must-revalidate'});
         console.log('GET-methode');
         console.log(path[0]);
         console.log(path[1]);
         res.end(JSON.stringify(power_state[(path[1])]));
       } else {
         res.statusCode = 404;
         res.end('not found');
         break;
       } 
     } else {
        res.statusCode = 404;
        res.end('not found');
        break;
      }
      break;
    case 'PUT':
      if (path.length == 2) {
        if (path[0] == "lounge" && path[1] >= 1 && path[1] <= 8) {
          res.writeHead(200, {'Content-Type': 'application/json; charset=utf-8', 'cache-control': 'max-age=0, no-cache, no-store, must-revalidate'});
          sendUDP(path[1],req);
          res.end();
        } else if (path[0] == "power" && path[1] >= 1 && path[1] <= 4) {
          res.writeHead(200, {'Content-Type': 'application/json; charset=utf-8', 'cache-control': 'max-age=0, no-cache, no-store, must-revalidate'});
          sendUDPPower(path[1],req);
          res.end();
        } else {
          res.statusCode = 404;
          res.end('not found');
          break;
        } 
      }else {
        res.statusCode = 404;
        res.end('not found');
        break;
      }
      break;

    default:
      res.statusCode = 405;
      res.end('Method not allowed');
  }
  
    }).listen(8080);
    console.log('Server running at Port 8080');








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
  if (data.length == 2 && data[0] == 0 && data[1] == 0)
    light_state[1].state = 'off'
  if (data.length == 2 && data[0] == 0 && data[1] == 1)
    light_state[1].state = 'on'
  if (data.length == 2 && data[0] == 1 && data[1] == 0)
    light_state[2].state = 'off'
  if (data.length == 2 && data[0] == 1 && data[1] == 1)
    light_state[2].state = 'on'
  if (data.length == 2 && data[0] == 2 && data[1] == 0)
    light_state[3].state = 'off'
  if (data.length == 2 && data[0] == 2 && data[1] == 1)
    light_state[3].state = 'on'
  if (data.length == 2 && data[0] == 3 && data[1] == 0)
    light_state[4].state = 'off'
  if (data.length == 2 && data[0] == 3 && data[1] == 1)
    light_state[4].state = 'on'
  if (data.length == 2 && data[0] == 7 && data[1] == 0)
    light_state[5].state = 'off'
  if (data.length == 2 && data[0] == 7 && data[1] == 1)
    light_state[5].state = 'on'
  if (data.length == 2 && data[0] == 4 && data[1] == 0)
    light_state[6].state = 'off'
  if (data.length == 2 && data[0] == 4 && data[1] == 1)
    light_state[6].state = 'on'
  if (data.length == 2 && data[0] == 5 && data[1] == 0)
    light_state[7].state = 'off'
  if (data.length == 2 && data[0] == 5 && data[1] == 1)
    light_state[7].state = 'on'
  if (data.length == 2 && data[0] == 6 && data[1] == 0)
    light_state[8].state = 'off'
  if (data.length == 2 && data[0] == 6 && data[1] == 1)
    light_state[8].state = 'on'
  //console.log(data);
  //console.log(data[0]);
  //console.log(data[1]);
  //console.log(light_state);
}


var client = dgram.createSocket('udp4');


sendUDP = function (id, state) {
  var state_old = 0;
  var id_old = 0;
  if (id == 1) id_old = 0;
  else if (id == 2) id_old = 1;
  else if (id == 3) id_old = 2;
  else if (id == 4) id_old = 3;
  else if (id == 5) id_old = 7;
  else if (id == 6) id_old = 4;
  else if (id == 7) id_old = 5;
  else if (id == 8) id_old = 6;



  //console.log(state);
  state.on('data', function (data) {
    //console.log(data);
    //console.log(data.toString);
    var obj = JSON.parse(data);
    //console.log(obj);
    if (obj.state == 'on') {
      state_old = 1;
    }
    var packet = new Buffer([0xA5,0x5A,id_old,state_old]);
    client.send(packet, 0, 4, 1337, 'licht.shack', function(err, bytes) { console.log(err) });
  });
}

sendUDPPower = function (id, state) {
  var state_old = 0;
  var id_old = 0;
  if (id == 1) id_old = 140;
  else if (id == 2) id_old = 141;
  else if (id == 3) id_old = 142;
  else if (id == 4) id_old = 143;


  //console.log(state);
  state.on('data', function (data) {
    //console.log(data);
    //console.log(data.toString);
    var obj = JSON.parse(data);
    console.log(obj);
    if (obj.state == 'on') {
      state_old = 1;
    }
  
    if (state_old == 1) {
      power_state[id].state = 'on'
    } else {
      power_state[id].state = 'off'
	}

    var packet = new Buffer([0xA5,0x5A,id_old,state_old]);
//    client.send(packet, 0, 4, 1337, 'licht.shack', function(err, bytes) { console.log(err) });
  });
}


