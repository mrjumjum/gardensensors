import * as d3 from 'd3';
import axios from 'axios';

// set the dimensions and margins of the graph
var margin = {top: 20, right: 20, bottom: 30, left: 50},
    width = 960 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

// parse the date / time "2018-04-13T08:24:40Z"
var parseTime = d3.timeParse("%Y-%m-%dT%H:%M:%SZ");

// set the ranges
var x = d3.scaleTime().range([0, width]);
var y = d3.scaleLinear().range([height, 0]);

// define the line
var valueline = d3.line()
    .x(function(d) { return x(d.date); })
    .y(function(d) { return y(d.moisture); });

// append the svg obgect to the body of the page
// appends a 'group' element to 'svg'
// moves the 'group' element to the top left margin
var svg = d3.select("body").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
  .append("g")
    .attr("transform",
          "translate(" + margin.left + "," + margin.top + ")");

var params = new URLSearchParams();
params.append('db', 'garden_measurements');
params.append("q", 'select * from light');

axios.post('http://18.236.166.41/query', params).then(response => {
	console.log(response);
	var data = response.data.results[0].series[0].values;
	console.log(data);
	data = data.map(d => ({
		date: parseTime(d[0]),
		moisture: d[4],
	}));

	console.log(data);

	x.domain(d3.extent(data, function(d) { return d.date; }));
 	y.domain([0, d3.max(data, function(d) { return d.moisture; })]);

	svg.append("path")
      		.data([data])
      		.attr("class", "line")
      		.attr("d", valueline);

	  // Add the X Axis
	  svg.append("g")
	      .attr("transform", "translate(0," + height + ")")
	      .call(d3.axisBottom(x));

	  // Add the Y Axis
	  svg.append("g")
	      .call(d3.axisLeft(y));
})
.catch(function (error) {
  console.log(error);
});
