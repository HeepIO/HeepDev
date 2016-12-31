import React from 'react';
import $ from 'jquery';

class RangeController extends React.Component {

	constructor(props) {
		super(props);
		this.displayMin = 7;
		this.displayMax = 60;

		this.state = {
			x: this.displayMin + (this.displayMax-this.displayMin)*(this.props.control['CurCtrlValue']/(this.props.control['HighValue']-this.props.control['LowValue'])),
			radius: 7
		}

		this.dragging = 0;

		this.lastPosition =  {top:  0,
							  left: 0};
		this.runningOffset = {top:  0,
							  left: 0};

		this.lastSentControlValue = this.props.control['CurCtrlValue'];
		this.newControlValue = this.props.control['CurCtrlValue'];
	}
	
	sendCommand(url) {
		this.calcNewControlValue();
	    var commandQueueString = [];
	    
	    if (this.lastSentControlValue == this.newControlValue){
	    	return
	    }

	    //SetCommand:destID,controlName,controlValue
	    
    	commandQueueString.push('SetCommand'+ ':' + 
    							this.props.ClientID + ',' +
    							this.props.control['ControlName'] + ',' +
								this.newControlValue + '\n');

	    console.log(commandQueueString);

	    const messagePacket = {command: commandQueueString};
	    $.ajax({
	      url: url,
	      type: 'POST',
	      data: messagePacket,
	      success: (data) => {
	        console.log("Commands Sent Successfully");
	        this.lastSentControlValue = this.newControlValue;
	      },
	      error: function(xhr, status, err) {
	        console.error(url, status, err.toString());
	        console.log('Hitting Commands sendDataToServer error')
	      }
	    });
	}

	calcNewControlValue() {
		this.newControlValue = Math.round((this.state['x'] - this.displayMin)/(this.displayMax-this.displayMin)*(this.props.control['HighValue']-this.props.control['LowValue']) + this.props.control['LowValue']);
	}

	onMouseDown(event) {
		console.log('starting drag');
		this.dragging = 1;
		this.lastPosition['left'] = event.screenX;
		
	}

	onMouseMove(event) {
		// The final drag event is always 0, whichthrows off tracking unless you catch and ignore it
		if ((event.screenX == 0 && event.screenY == 0) || !this.dragging){
			return;
		}

		this.runningOffset['left'] = event.screenX - this.lastPosition['left']  ;
		var setPosition = this.state['x'] + this.runningOffset.left;
		if (setPosition < this.displayMin){
			setPosition = this.displayMin;
		}
		else if (setPosition > this.displayMax){
			setPosition = this.displayMax;
		}

		this.lastPosition['left'] = event.screenX;
		this.setState( {x: setPosition});
		
		this.sendCommand(this.props.url.concat('/api/commands'));
	}

	onWheel(event) {
		event.preventDefault(); 

		if (event.deltaY < 0){
			var newVal = this.state.x + this.displayMin

			if (newVal > this.displayMax){
				newVal = this.displayMax;
			}

			this.setState({x: newVal });
		}
		else {
			var newVal = this.state.x - this.displayMin;
			if (newVal < this.displayMin){
				newVal = this.displayMin;
			}
			this.setState({x: newVal});
		} 

		this.sendCommand();
	}

	render() {

		var styles = {
			button: {
				display: 'block',
				draggable: false,
				verticalAlign: 'center',
				marginRight: 'auto',
				height: 35
			}
		};

		var inputs = {
			button: {
				style: styles.button,
				onMouseUp : (event) => {this.dragging = 0;},
				onMouseLeave : (event) => {this.dragging = 0;},
				onMouseMove : (event) => {this.onMouseMove(event)},
				onWheel : (event) => this.onWheel(event),
			},
			rangeContainer: {
				width: 68,
				height: 35,
				viewBox: '0 0 68 35'
			},
			unselected:{
				strokeWidth: 1,
				stroke: 'grey',
				x1: this.displayMin,
				x2: this.displayMax,
				y1: 11,
				y2: 11,
			},
			selected:{
				strokeWidth: 2,
				stroke: 'black',
				x1: this.displayMin,
				x2: this.state.x,
				y1: 11,
				y2: 11,
			},
			dragDot: {		
				onMouseEnter : () => this.setState({radius: 7.5}),
				onMouseLeave : () => this.setState({radius: 7}),
				onMouseDown : (event) => {this.onMouseDown(event);},
				onMouseUp : (event) => {this.dragging = 0;},
				cx: this.state.x,
				cy: 11,
				r: this.state.radius,
				fill: "black"
			},
			text: {
				x: this.state.x,
				y: 14,
				fontFamily: "Verdana",
				fontSize: 8.5,
				fill: '#e1e3e8',
				textAnchor: 'middle',
				pointerEvents: 'none'

			}
		};

		return  <div {...inputs.button}>
					<svg {...inputs.rangeContainer}>
						<line {...inputs.unselected}/>
						<line {...inputs.selected}/>
						<circle {...inputs.dragDot} ref='dragDot'/>
						<text {...inputs.text}> {this.lastSentControlValue} </text>
					</svg>
				</div>
            	
	}


}

export default RangeController;