import { combineReducers } from 'redux'
import Immutable from 'immutable'
// import 'babel-polyfill'
import { initialState } from '../index.jsx'

export default function(state = initialState, action) {
  switch (action.type) {
    case 'ADD_MEMORY_DUMP' :


      var newData = Immutable.Map(state.devices).toJS();//.set('data', analyticsList).toJS();

    	var pushPacket = action.MOP.analytics;
      pushPacket.deviceID = action.deviceID;
      var thisDeviceData = newData[action.deviceID];

      if (thisDeviceData != undefined) {

        var analyticsList = Immutable.List(thisDeviceData).push(pushPacket).toJS();

      } else {
        console.log("Start device");

        var analyticsList = Immutable.List([]).push(pushPacket).toJS();
        
      }

      console.log("reducer: ", state);

      var newDeviceData = Immutable.Map(state.devices).set(action.deviceID, analyticsList).toJS();

      return Immutable.Map(state).set('devices', newDeviceData)

    case 'SET_MAX_TIME_RANGE' :

    	return Immutable.Map(state).set('maxTime', action.time).toJS();

    case 'SET_MIN_TIME_RANGE' :

    	return Immutable.Map(state).set('mInTime', action.time).toJS();

    default:
      return state
  }
}
