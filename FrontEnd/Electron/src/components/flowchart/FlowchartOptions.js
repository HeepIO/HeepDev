import React from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import { withRouter } from 'react-router-dom'
import * as Actions from '../../redux/actions_classic'
import fileDownload from 'react-file-download'

import { withStyles } from 'material-ui/styles'

import Add from 'material-ui-icons/Add'
import FileUpload from 'material-ui-icons/FileUpload'
import FileDownload from 'material-ui-icons/FileDownload'
import Remove from 'material-ui-icons/Remove'
import PhotoCamera from 'material-ui-icons/PhotoCamera'
import SwitchCamera from 'material-ui-icons/SwitchCamera'
import { Button, Tooltip, Modal, Paper, Menu, MenuItem, ListItemSecondaryAction, IconButton }  from 'material-ui'

import GenericTextInput from '../utilities/GenericTextInput'

var mapStateToProps = (state) => ({
  scale: state.flowchart.scale,
  snapshots: state.stateSnapshots
})

const styles = theme => ({
  paper: {
    position: "absolute",
    backgroundColor: theme.palette.background.paper,
    boxShadow: theme.shadows[5],
    padding: theme.spacing.unit * 4,
    minWidth: 500,
    top: '50%',
    left: '50%',
    transform: 'translate(-50%, -50%)'
  }
});

class FlowchartOptions extends React.Component {
	state = {
		newSnapshotModal: false,
		newSnapshotName: '',
		anchorEl: null
	}

	render() {
		return (
			<div
				id='flowchartOptions'
				style={{
					position:'fixed',
					display: 'flex',
					bottom:  this.props.theme.spacing.unit,
					right: this.props.theme.spacing.unit
			}}>
				{this.newSnapshotButton()}
				{this.selectSnapshotButton()}
				{this.zoomOptions()}

				{this.newSnapshotModal()}
			</div>
		)
	}

	newSnapshotModal = () => (
		<Modal
			  open={this.state.newSnapshotModal}
			  onClose={() => {
			  	this.setState({
			  		newSnapshotModal: false,
			  		newSnapshotName: ''
			  	});
			}}>
			  <Paper className={this.props.classes.paper}>
			  	<GenericTextInput 
			  	  value={this.state.newSnapshotName}
			  	  width='100%'
			  	  title='New Snapshot Name'
			  	  onChange={(value) => {this.setState({newSnapshotName: value})}}
			  	/>
			  	<div style={{
			  		display: 'flex',
			  		flexDirection: 'row-reverse',
			  		width: '100%'
			  	}}>
			  		<Button 
			  			variant='raised' 
			  			color='primary'
			  			onClick={() => {
			  				this.props.saveSnapshot(this.state.newSnapshotName)
			  				this.setState({newSnapshotModal: false})
			  			}}
			  		>
			  			Save
			  		</Button>
			  	</div>
			  </Paper>
		</Modal>
	)

	selectSnapshotButton = () => (
		<Tooltip id="tooltip-snapshot-return"
	            title='Return to Snapshot'
	            placement="top">
            <div>
				<Button
					mini
					variant="fab"
					color="primary"
					aria-label="snapshot-return"
					onClick={(event) => this.setState({ anchorEl: event.currentTarget })}
					style={{marginRight: this.props.theme.spacing.unit}}
				>
					<SwitchCamera/>
				</Button>

				<Menu
				  id="simple-menu"
				  anchorEl={this.state.anchorEl}
				  open={Boolean(this.state.anchorEl)}
				  onClose={() => this.setState({anchorEl: null})}
				>
				  <MenuItem onClick={() => console.log('Upload Snapshot')}>
				  		<FileUpload style={{marginRight: this.props.theme.spacing.unit}}/>
				  		Upload Snapshot JSON
				  </MenuItem>
				  <MenuItem onClick={() => this.setState({newSnapshotModal: true})}>
				  		<Add style={{marginRight: this.props.theme.spacing.unit}}/>
				  		New Snapshot
				  </MenuItem>
				  {Object.keys(this.props.snapshots).map((thisSnapshot) => (
				  	<MenuItem key={thisSnapshot} onClick={() => this.props.returnToSnapshot(thisSnapshot)}>
				  		{this.props.snapshots[thisSnapshot].name}
				  		<ListItemSecondaryAction>
							<IconButton aria-label="Export" onClick={ () => {
								console.log('export ', thisSnapshot);
								fileDownload(JSON.stringify(this.props.snapshots[thisSnapshot], null, 2), this.props.snapshots[thisSnapshot].name + '_Snapshot.json')
							}}>
								<FileDownload />
							</IconButton>
						</ListItemSecondaryAction>
				  	</MenuItem>
				  	))}
				</Menu>

			</div>
		</Tooltip>
	)

	newSnapshotButton = () => (
		<Tooltip id="tooltip-save-state"
	            title='Save State Snapshot'
	            placement="top">
			<Button
				mini
				variant="fab"
				color="primary"
				aria-label="save-state"
				onClick={() => this.setState({newSnapshotModal: true})}
				style={{marginRight: this.props.theme.spacing.unit}}
			>
				<PhotoCamera/>
			</Button>
		</Tooltip>
	)

	zoomOptions = () => (
		<Tooltip id="tooltip-range"
	            title={Math.round(this.props.scale * 100) + "%"}
	            placement="top">
		    <div>
				<Button
					mini
					variant="fab"
					color="primary"
					aria-label="zoom-out"
					onClick={() => this.props.zoomOut()}
					style={{marginRight: this.props.theme.spacing.unit}}
				>
					<Remove/>
				</Button>
				<Button
					mini
					variant="fab"
					color="primary"
					aria-label="zoom-in"
					onClick={() => this.props.zoomIn()}
				>
					<Add/>
				</Button>
			</div>
		</Tooltip>
	)
}

var mapDispatchToProps = (dispatch) => {
  return bindActionCreators(Actions, dispatch)
}

export default withRouter(connect(mapStateToProps, mapDispatchToProps)(withStyles(styles, {withTheme: true})(FlowchartOptions)))
