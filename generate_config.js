/**
 * Created by JonXP on 2/24/2017.
 */
var fs = require("fs");
var args = process.argv.slice(2);
var filename = args.length > 0 ? args.join(' ') : 'config.json';

var config = JSON.parse(fs.readFileSync(filename));
var mustache = require('mustache');
var tpl = fs.readFileSync("led_config.h.mustache").toString();

function ledData(pos) {
  return {
    x: 0,
    x8: 0,
    y: 0,
    y8: 0,
    pos: pos,
    theta: 0,
    r: 0,
    r8: 0,
    ring: 0,
    ringPos: 0
  }
}

var leds = [];

for (var i=0; i < config.ledCount; i++) {
  leds.push(ledData(i));
}

function map(val, fromMin, fromMax, toMin, toMax) {
  return (val - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

var maxRadius = config.rings.reduce(function(acc, ring) {
  if (ring.radius > acc)
    acc = ring.radius;
  return acc;
}, 0);

config.rings.forEach(function(ring, ringIdx) {
  arcLengthDegrees = 360 / ring.leds.length;
  arcLengthRads = (2 * Math.PI) / ring.leds.length;
  ring.leds.forEach(function(ledPos, ringPos){
    var data = leds[ledPos];
    data.ringPos = ringPos;
    data.r = ring.radius;
    data.r8 = Math.round(map(data.r, 0, maxRadius, 0, 127));
    data.theta = Math.round(map(arcLengthDegrees * ringPos, 0, 360, 0, 255));
    data.ring = ringIdx;
    data.x = Math.round(Math.cos(arcLengthRads * ringPos) * ring.radius);
    data.y = Math.round(Math.sin(arcLengthRads * ringPos) * ring.radius);
    data.x8 = Math.round(map(data.x, -1 * maxRadius, maxRadius, -127, 127));
    data.y8 = Math.round(map(data.y, -1 * maxRadius, maxRadius, -127, 127))
  });
});

console.log(leds);
config.leds = leds;

var output = mustache.render(tpl, config);

console.log(output);
fs.writeFileSync("led_config.h", output);