var adn = require('../adn'),
	keys = require('../keys')

var relation = []

exports.list = function(req, res){
 	
 	console.log('LIST')
 	var lat = parseInt(req.body['0'])
 	var lon = parseInt(req.body['1'])

 	console.log(req.body)
 	adn.searchPlace(parseFloat(lat/10000000), parseFloat(lon/10000000), keys.count, function (err, places){

 		if (err) res.send(JSON.stringify({0:'error'}))
 		else {

			var dict = {}
			relation = []
			console.log('Places '+places)
			for (var i = 0; i < places.length; i++){

				var place = places[i]
				var name = place.name

				if (name.length > 15) name = name.substring(0, 15)+'...'

				dict[i] = name
				relation[i] = place.factual_id
			}
			if (!dict[0]) dict[0] = 'Nothing found'

			res.send(JSON.stringify(dict))
			console.log(relation)
 			
 		}
 	})
}

//MORE FUNCTION...
exports.more = function (req, res){

	var c = relation.length
	var lat = parseInt(req.body['0'])
 	var lon = parseInt(req.body['1'])

	adn.searchPlace(parseFloat(lat/10000000), parseFloat(lon/10000000), c+keys.count, function (err, places){

 		if (err) res.send(500)
 		else {

			var dict = {}
			
			for (var i = c; i < places.length; i++){

				var place = places[i]
				var name = place.name

				if (name.length > 15) name = name.substring(0, 15)+'...'

				dict[i] = name
				relation[i] = place.factual_id
			}

			res.send(JSON.stringify(dict))
			console.log(relation)
 			
 		}
 	})

}


exports.checkin = function(req, res){

	console.log('CHECK IN')
	var i = req.body['0']

	var id = relation[i]

	if (!id) res.send('redo /list. Lost cache...')
	else {
		adn.checkinPlace(id, function(err, data){

			if (err) res.send(JSON.stringify({0:'error'}))
			else if (data){

				res.send(JSON.stringify({0:'ok'}))
			}
		})
	}	
}