import React from 'react'
import { Grid } from 'material-ui'

const VideoBackgroundCard = (componentProps, wrappedComponent) => {

  const inputs = {
    outerContainer: {
      // backgroundImage: componentProps.imagePath !== undefined ? `url(${componentProps.imagePath})` : '',
      backgroundRepeat: 'no-repeat',
      backgroundSize:'cover',
      maxWidth: '100%',
      margin: 0,
      position: 'relative',
      overflow: 'hidden'
    }
  };

  return(
    <div style={{...inputs.outerContainer, ...componentProps}}>
      <div style={{position: 'absolute', top: 0, left: 0, zIndex: 1, width: '100%'}}>
        <video autoplay loop muted width='100%' id='iiotVid'>
          <source src={componentProps.videoSrc} type="video/mp4"/>
          Your browser does not support the video tag.
        </video>
      </div>
      <div style={{position: 'absolute', top: 0, left: 0, zIndex: 2, backgroundColor: 'rgba(0,0,0, 0.5)' }}>
        {wrappedComponent}
      </div>
    </div>
  );
};

export default VideoBackgroundCard
