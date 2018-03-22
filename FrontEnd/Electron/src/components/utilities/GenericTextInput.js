import React from 'react'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'

import Input, { InputLabel } from 'material-ui/Input';
import { FormControl, FormHelperText } from 'material-ui/Form';


export default class GenericTextInput extends React.Component {

	render() {
	    return (
	    	<FormControl style={{
			    width: this.props.width,
			    margin: 4
			  }}>
	    	  <InputLabel htmlFor="text-input">{this.props.title}</InputLabel>
	    	  <Input id="name-input"  
	    	  	value={this.props.value}
	    	    onChange={ (event) => {this.props.onChange(event.target.value)}}/>
	    	  <FormHelperText>{this.props.helperText}</FormHelperText>
	    	</FormControl>

	    )
	}
}