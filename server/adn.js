var request = require('request'),
	keys = require('./keys')

exports.searchPlace = function (lat, lon, count, callback){

	var url = 'https://alpha-api.app.net/stream/0/places/search?latitude='+lat+'&longitude='+lon+'&count='+count+'&radius=1000'
	request({

		url:url,
		headers: {

			'Authorization': 'Bearer '+keys.token
		}
	}, function (err, res, body){

		if (err) callback(err, null)
		else {	
			var r = JSON.parse(body)
			callback(null, r.data)
		}
	})

}


exports.checkinPlace = function (id, callback){

	var url = 'https://alpha-api.app.net/stream/0/channels/'+keys.channel_id+'/messages'
	var payload = {

		annotations:[
			{
				type:"net.app.core.checkin",
				value:{
					"+net.app.core.place":{
						factual_id:id
					}
				}
			}
		],
		machine_only:true
	}
	request({

		url:url,
		method:'POST',
		body: JSON.stringify(payload),
		headers: {

			Authorization: 'Bearer '+keys.token,
			'Content-Type': 'application/json'
		}
	}, function (err, res, body){

		if (err) callback(err, null)
		else {

			console.log(body)
			var r = JSON.parse(body)
			callback(null, r.data)
		}
	})
}

exports.text = function (text, )