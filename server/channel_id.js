var adn = require('./adn')

console.log('Getting channels...')
adn.getChannels(function(err, channels){

	
	for (var i = 0; i< channels.length;i++){

		var channel = channels[i]
		
		if (channel.type == 'net.app.ohai.journal'){

			console.log('Your ohai channel id is: '+channel.id)
		}
	}
})