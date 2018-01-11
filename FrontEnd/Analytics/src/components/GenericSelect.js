import React from 'react'
import {HashRouter as Router, Route} from 'react-router-dom'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import * as Actions from '../redux/actions'
import {FormGroup, FormControl, ControlLabel, option} from 'react-bootstrap';

export default class GenericSelect extends React.Component {
	
	constructor(props) {
		super(props);
	}

	render() {
		
	    var options = [];

	    for (var i in this.props.options) {
	    	options.push (<option 
	    		key={this.props.keyID + this.props.options[i]}
	    		value={this.props.options[i]}>{this.props.options[i]}
	    		</option>)
	    }

	    return(<FormGroup controlId="formControlsSelect">
	              <ControlLabel style={{color:"black"}}>{this.props.title}</ControlLabel>
	              <FormControl 
	                componentClass="select" 
	                defaultValue={this.props.defaultValue}
	                onChange={
	                  (change) => {this.props.onChange(change.target.value);}
	                }>
	                {options}
	              </FormControl>
	            </FormGroup>);
	    
	}
}