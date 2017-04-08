import assert from 'assert';
import * as connect from './HeepConnections';

var emptyState = {
    clients: {clientArray: []},
    positions: {},
    controls: {controlStructure:{}},
    vertexList: {},
    icons: {},
    url: ''
  }

describe('HeepConnections.js', () => {
	describe('ResetMasterState', () => {
		it('Returns an empty masterState', () => {
		  	assert.equal(JSON.stringify(emptyState), JSON.stringify(connect.ResetMasterState()));
		});
	});

	describe('GetMasterState', () => {
		it('Returns masterState', () => {
			connect.ResetMasterState();
		  	assert.equal(JSON.stringify(emptyState), JSON.stringify(connect.GetCurrentMasterState()));
		});
	});

	describe('FindGateway', () => {
		it('Returns an array with 3 values', () => {
			var ip = connect.findGateway();
		  	assert.equal(3, ip.length);
		});
	});

});