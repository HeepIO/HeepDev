import React from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import { withRouter } from 'react-router-dom'
import * as Actions from '../../redux/actions_classic'
import * as Draggable from 'gsap/Draggable'

import Device from './DevicePaper'
import Vertex from './Vertex'
import DeviceDetailsPanel from '../heep/DeviceDetailsPanel'

import Add from 'material-ui-icons/Add'
import Remove from 'material-ui-icons/Remove'
import { Button }  from 'material-ui'

var mapStateToProps = (state) => ({
  deviceArray: Object.keys(state.devices),
  vertexList: state.vertexList,
  scale: 0.5
})


class Flowchart extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			hoverRefresh: false
		}
	};

	componentDidMount() {
		this.initializeDrag();
	}

	flowchartOptions() {

		return (
			<div style={{position:'absolute', top: 10, left: 10}}>
				<Button 
					variant="fab" 
					color="primary" 
					aria-label="zoom-out" 
					onClick={() => console.log('zoom out')}
				>
					<Remove/>
				</Button>
				<Button 
					variant="fab" 
					color="primary" 
					aria-label="zoom-in" 
					onClick={() => console.log('zoom in')}
				>
					<Add/>
				</Button>
				
			</div>
		)
	}

	drawVertices() {

		const inputs = {
			vertexSVGSpace: {
        	id: 'vertexSVGSpace',
				style: {
					position: 'absolute',
					width: 3000,
					height: 3000,
					viewBox: '0 0 1000 1000',
          			top: 0,
          			left: 0,
				},
			}
		}

		return (
			<svg {...inputs.vertexSVGSpace}>
				{Object.keys(this.props.vertexList).map((thisVertexKey) => {
					if (thisVertexKey != 'selectedOutput'){
              return <Vertex key={thisVertexKey} vertexID={thisVertexKey} />
					}
				})}
			</svg>
		)
	};

  initializeDrag () {
    Draggable.create(".devicePaper", {
      type: "x,y",
      allowContextMenu: true,
      onDrag: () => this.props.updateDragging()
    });
  }

	render() {

		const inputs = {
			flowchart: {
				style: {
					height: 3000,
					width: 3000,
					backgroundColor: '#e7e7e7',
					overflow: 'auto'
				}
			},
			refresh: {
			  width: 45,
			  height: 45,
			  type:"image/svg+xml",
			  data: './dist/assets/svg/refresh.svg',
			  style: {
			  	position: 'absolute',
			  }
			},
			refreshContainer: {
				style: {
					width: 50,
					height: 50,
					top: 2,
					left: 2,
					backgroundColor: this.state.hoverRefresh ? "rgba(0,0,0,0.1)" : "rgba(0,0,0,0)",
					cursor: "pointer",
					position: 'relative'
				}
			},
			refreshButton: {
				style: {
					width: 50,
					height: 50,
					backgroundColor: this.state.hoverRefresh ? "rgba(0,0,0,0.1)" : "rgba(0,0,0,0)",
					cursor: "pointer",
					position: 'absolute'
				},
				onClick: () => {
					console.log("clicked");
					this.props.refreshFlowchart();
				},
        		onMouseEnter: () => this.setState({hoverRefresh: true}),
        		onMouseLeave: () => this.setState({hoverRefresh: false})
			},
      		deviceContainer: {
		        style: {
		          position: 'absolute',
		          width: '100%',
		          height: '100%',
		          background: 'none',
		          pointerEvents: 'none',
		          transform: 'scale(' + this.props.scale + ')',
		          transformOrigin: 'top left'
		        }
		    }
		}



		return (
	      	<div {...inputs.flowchart} ref="flowchart">
		        {this.drawVertices()}
		        <div {...inputs.deviceContainer}>
	  				{this.props.deviceArray.map((thisDevice) => (
			            <div className="devicePaper" id={thisDevice} key={thisDevice}>
			  				<Device DeviceID={thisDevice}/>
			            </div>
	  				))}
	  				
	  				{this.flowchartOptions()}
		        </div>
		        <DeviceDetailsPanel/>
			</div>
		);

	}
}

var mapDispatchToProps = (dispatch) => {
  return bindActionCreators(Actions, dispatch)
}

export default withRouter(connect(mapStateToProps, mapDispatchToProps)(Flowchart))
