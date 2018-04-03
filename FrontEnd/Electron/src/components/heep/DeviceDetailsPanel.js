import React from 'react';

import { connect }            from 'react-redux'
import { bindActionCreators } from 'redux'
import { withRouter }         from 'react-router-dom'
import * as actions           from '../../redux/actions_classic'

import { withTheme } from 'material-ui/styles';
import { Drawer, Button, Divider, Paper, Typography, IconButton } from 'material-ui';
import List, { ListItem, ListItemIcon, ListItemText, ListSubheader } from 'material-ui/List';
import { Close } from 'material-ui-icons'

import DetailsPanelControlBlock from './DetailsPanelControlBlock'

var mapStateToProps = (state) => ({
  deviceID:  state.detailsPanelDeviceID,
  device: state.devices[state.detailsPanelDeviceID],
  controls: state.detailsPanelDeviceID == null ? {} : extractControls(state)
})

const extractControls = (state) => {

  const inputs = state.controls.controlStructure[state.detailsPanelDeviceID].inputs;
  const outputs = state.controls.controlStructure[state.detailsPanelDeviceID].outputs;

  const allControls = inputs.concat(outputs);

  return allControls

}

class DeviceDetailsPanel extends React.Component {

  deviceDetails() {
    return (
      <div style={{
        width: this.props.deviceID != null ? 250 : 0, 
        marginTop: 70,
        padding: this.props.theme.spacing.unit,
        position: 'relative'
      }}>


        {this.detailsTitle()}

        <IconButton 
          onClick={() => this.props.setDetailsPanelDeviceID(null)}
          size='small'
          style={{
            position: 'absolute', 
            right: 4, 
            height: 24,
            width: 24,
            top: 4, 
            fontSize: 16,
            cursor: 'pointer'
          }}>
            <Close 
              style={{
                fontSize: 16
              }}/>
          </IconButton>

        <List 
          disablePadding 
          dense
          subheader={<ListSubheader component="div" style={{padding: 0}}>Device Details</ListSubheader>}>
          
          <Divider/>

          {Object.keys(this.props.device).map((field) => (
            this.deviceIdentity(field, this.props.device[field])
          ))}
        </List>
        {this.listControls()}
        <div style={{height: 500}}/>
      </div>
    )
  }

  listControls() {
    return (
      <List 
        disablePadding 
        dense
        subheader={
          <ListSubheader component="div" style={{padding: 0}}>
            Controls
          </ListSubheader>}>
        
        <Divider/>

        {this.props.controls.map((controlID) => (
          <DetailsPanelControlBlock key={controlID} controlID={controlID}/>
        ))}

      </List>
    )
  }

  detailsTitle() {

    var inputs = {
      primary: {
        style: {
          maxHeight: '100%',
          maxWidth: '100%'
        },
        type:"image/svg+xml",
        data: "/assets/svg/" + this.props.device.iconName + ".svg"
      }
    }

    return (
      <ListItem style={{paddingRight: 0}}>
        <ListItemIcon>
            <object {...inputs.primary}/>
        </ListItemIcon>
        <ListItemText primary={this.props.device.name} />
      </ListItem>
    )
  }

  deviceIdentity(field, value) {

    return (
      <ListItem key={field} > 
        <ListItemText primary={field + ' : '} />
        <Typography >
          {value.toString()}
        </Typography>
      </ListItem>
    )
  }

  render() {

    return (  
        <Drawer anchor="right" variant='permanent' open={this.props.deviceID != null} >
          {this.props.deviceID != null && (this.deviceDetails())}
          
        </Drawer>
    );
  }
}

var mapDispatchToProps = (dispatch) => {
  return bindActionCreators(actions, dispatch)
}

export default withRouter(connect(mapStateToProps, mapDispatchToProps)(withTheme()(DeviceDetailsPanel)))