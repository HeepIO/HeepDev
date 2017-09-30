import React from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import * as Actions from '../redux/actions'
import fileDownload from 'react-file-download'
import JSZip from 'jszip'

var mapStateToProps = (state, ownProps) => ({
  signal: state.signals[ownProps.id],
  files: state.files
})

class DownloadConfig extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      hover: false
    }
  }

  handleDownload() {

    var zip = new JSZip();
    zip.file('Autogenerated.h', 'String SignalID = "'+ this.props.signal.id + '";');

    var name = 'PaperSignals_' + this.props.signal.content.name.split(' ').join('_');
    var nameZip = name + '.zip';
    var nameIno = name + '.ino';

    for (var filename in this.props.files) {

      if (filename == 'PaperSignals.ino') {

        zip.file(nameIno, this.props.files[filename]);

      } else {

        zip.file(filename, this.props.files[filename]);
      }

    }

    zip.generateAsync({type:"blob"})
    .then(function(content) {

        fileDownload(content, nameZip);
    });
  }

  render() {

  	const styles = {
        container : {
          position: "absolute",
          bottom: 0,
          right: 0,
          height: 45,
          width: 50,
          margin: 10,
          display: "flex",
          flexDirection: "row",
          borderWidth: 2,
          color: "black",
          backgroundColor: this.state.hover ? "#999" : "rgba(255,255,255,0)",
          alignItems: "center",
          justifyContent: "center",
          cursor: "pointer"
        },
        glyph: {
        	fontSize: 20
        }
      };

    const inputs = {
      container: {
        style: styles.container,
        onMouseEnter: () => this.setState({hover: true}),
        onMouseLeave: () => this.setState({hover: false}),
        onClick: () => {this.handleDownload()}
      },
      glyph: {
      	style: styles.glyph,
      	className: "glyphicon glyphicon-download-alt"
      }
    }
    
    return (<div {...inputs.container}> 
    			     <span {...inputs.glyph}> </span>
            </div>
    );
  }
}

var mapDispatchToProps = (dispatch) => {
  return bindActionCreators(Actions, dispatch)
}

export default connect(mapStateToProps, mapDispatchToProps)(DownloadConfig)