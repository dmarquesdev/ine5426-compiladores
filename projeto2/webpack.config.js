var webpack = require('webpack');

module.exports = {
	entry: ['./app/app.js'],
	output: {
		path: './js',
		filename: 'bundle.js'
	},

	module: {
		loaders: [
			{
				loader: 'babel-loader',
				test: /\.jsx?$/,
				exclude: /(node_modules|bower_components)/,
				query: {
					presets: ['stage-0', 'react', 'es2015']
				}
			}
		]
	}
}
