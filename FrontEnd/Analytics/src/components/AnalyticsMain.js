import React from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import * as Actions from '../redux/actions'

import AnalyticsList from './AnalyticsList'

export default class AnalyticsMain extends React.Component {
  constructor(props) {
    super(props);
  }

  render () {

    var inputs = {
      builder: {
        style: {
          backgroundColor: "white",
          fontFamily: "-apple-system,BlinkMacSystemFont,Segoe UI,Roboto,Oxygen-Sans,Ubuntu,Cantarell,Helvetica Neue,sans-serif"
        }
      },
      contents: {
        style: {
          width: "80%",
          margin: "auto",
        }
      },
      spacer: {
        style: {
          height: 20
        }
      },
      lineChart: {
        deviceID: 66051,
        filterQuery: 'timeStamp',
        timeStamp: new Date(Date.UTC(2018, 1, 1, 0, 0, 0))
      },
      displayAnalytics: {
        deviceID: 66051
      }
    }


    return (
      <div {...inputs.builder}>
        <div {...inputs.contents}>
          <div {...inputs.spacer}/>
          <AnalyticsList {...inputs.displayAnalytics}/>
          <div {...inputs.spacer}/>
        </div>
     </div>
    );
  }
}
