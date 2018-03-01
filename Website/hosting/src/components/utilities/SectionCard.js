import React from 'react';
import Typography from 'material-ui/Typography';
import Grid from 'material-ui/Grid';

class SectionCard extends React.Component {

  headerText(headerText) {
    return (
      <Typography variant="headline" align="center" color='inherit'>
        {headerText}
      </Typography>
    )
  }

  subText(subText) {
    return (
      <Typography style={{paddingTop: 10}} component="p" align="center" color='inherit'>
        {subText}
      </Typography>
    )
  }

  render() {

    var inputs = {
      outerContainer: {
        style: {
          backgroundColor: this.props.backgroundColor,
          backgroundImage: `url(${this.props.imagePath})`,
          backgroundRepeat: 'no-repeat',
          backgroundSize:'cover',
          minHeight: this.props.containerHeight,
          maxWidth: '100%',
          paddingTop: this.props.paddingTop,
          paddingBottom: this.props.paddingBottom,
          margin: 0
        }
      }
    }

    return (
      <Grid container {...inputs.outerContainer} direction ='column' justify="center" >
        <Grid container spacing={0} justify="center" >
          <Grid item xs={8}>
            {this.headerText(this.props.headerText)}
            {this.subText(this.props.subText)}
          </Grid>
        </Grid>
      </Grid>
    )
  }
}

export default SectionCard