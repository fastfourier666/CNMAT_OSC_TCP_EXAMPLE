// A simple TCP/OSC server.

var osc = require ('osc');
var net = require ('net');
var util = require ('util');

const LISTEN_PORT = 12345;				// port to listen on
const LISTEN_ADDRESS = "0.0.0.0";		// all interfaces

var serverPort = net.createServer (function(socket) {
	var tcpServerPort = new osc.TCPSocketPort ({
		socket: socket
	});

	tcpServerPort.on ("open", function () {
		console.log ('opened');
	})

	tcpServerPort.on ("error", function (err) {
		console.error (err);
	});

	tcpServerPort.on ("message", function (msg) {
		if (msg.address=='/hello') {
			tcpServerPort.send ({
				address : `/hello_reply`,
				args : [
				{
					type : `i`,
					value : 667
				}
				]
			});
		}
		console.log (`Message from ${socket.remoteAddress}:${socket.remotePort} - ${util.inspect (msg)}`);

	});
})

// start listening 
serverPort.listen(LISTEN_PORT, LISTEN_ADDRESS, function () {
	console.log (`listening on ${serverPort.address().address} : ${serverPort.address().port}`);
});
