export const simulationDevice = { 
  devices: { 
    '98765': { 
      DeviceID: 98765,
      IPAddress: '1.1.1.1',
      DeviceName: 'Simulation Device' 
    },
     deviceArray: [ 98765 ] 
   },
  positions: { 
    '98765': { 
      device: { 
        top: 0, 
        left: 0 
      },
    '98765.0': { 
      top: 59, 
      left: 10, 
      index: 1 
    },
    '98765.1': { 
      top: 59, 
      left: 250, 
      index: 1 
    }
  } 
},
  controls: { 
    controlStructure: { '98765': { inputs: [ '98765.0' ], outputs: [ '98765.1' ] } },
     connections: { '98765.0': [], '98765.1': [] },
     '98765.0':
      { ControlID: 0,
        ControlValueType: 0,
        ControlDirection: 0,
        LowValue: 0,
        HighValue: 1,
        CurCtrlValue: 0,
        ControlName: 'Input',
        deviceID: 98765 },
     '98765.1':
      { ControlID: 1,
        ControlValueType: 0,
        ControlDirection: 1,
        LowValue: 0,
        HighValue: 1,
        CurCtrlValue: 1,
        ControlName: 'Output',
        deviceID: 98765 } },
  vertexList: {},
  icons:
   { '98765': 'none',
     none: '<?xml version="1.0" encoding="UTF-8" standalone="no"?>\r\n<!-- Created with Inkscape (http://www.inkscape.org/) -->\r\n\r\n<svg\r\n   xmlns:dc="http://purl.org/dc/elements/1.1/"\r\n   xmlns:cc="http://creativecommons.org/ns#"\r\n   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"\r\n   xmlns:svg="http://www.w3.org/2000/svg"\r\n   xmlns="http://www.w3.org/2000/svg"\r\n   version="1.1"\r\n   width="198.63"\r\n   height="235.75999"\r\n   viewBox="0 0 198.63 235.75999"\r\n   id="svg2">\r\n  <defs\r\n     id="defs4" />\r\n  <metadata\r\n     id="metadata7">\r\n    <rdf:RDF>\r\n      <cc:Work\r\n         rdf:about="">\r\n        <dc:format>image/svg+xml</dc:format>\r\n        <dc:type\r\n           rdf:resource="http://purl.org/dc/dcmitype/StillImage" />\r\n        <dc:title></dc:title>\r\n      </cc:Work>\r\n    </rdf:RDF>\r\n  </metadata>\r\n  <g\r\n     transform="translate(0,-816.60215)"\r\n     id="layer1">\r\n    <path\r\n       d="m 16.827608,1003.8799 95.459422,47.9823 c 1.68109,-9.0605 1.12345,-20.836 9.09137,-22.981 -0.37524,-29.55123 15.14538,-18.692 26.76904,-17.9302 10.99806,0.2838 23.45462,0.1315 27.27412,-10.1015 l 0,-5.05078 c -1.17069,-3.36835 -2.25917,-13.59662 5.55584,-16.66752 -0.48691,-3.58063 -0.91797,-7.26006 -3.53554,-7.07107 l 0,-2.46225 c 0.37034,-0.22796 5.44906,-1.40635 4.54569,-7.1342 -2.42853,1.62144 -3.71595,-9.19145 -0.50507,-10.6066 30.34837,-7.17328 12.61761,-17.73826 -1.51523,-43.94163 -5.79974,-5.21912 -3.91764,-10.43825 1.01015,-15.65737 -2.33436,-12.49956 -7.27001,-26.73331 -13.63705,-41.92133 -13.56886,-27.39226 -94.145252,-52.46138 -137.885842,-12.12183 -25.53449,19.81017 -37.80786,46.25091 -21.71827,86.87312 19.15637,34.71088 18.09239,58.83014 9.09137,78.79186 z"\r\n       id="path2998"\r\n       style="fill:#000000;fill-opacity:1;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:round;stroke-opacity:1" />\r\n    <g\r\n       id="text3768"\r\n       style="font-size:20px;font-style:normal;font-variant:normal;font-weight:900;font-stretch:normal;line-height:125%;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;font-family:Cooper Std;-inkscape-font-specification:Cooper Std Heavy">\r\n      <path\r\n         d="m 89.598801,936.39728 -4.003907,0 c -0.130251,-7.22651 0.195269,-12.63015 0.976563,-16.21094 0.846309,-3.58067 2.962192,-8.49603 6.347656,-14.74609 3.450467,-6.31503 5.696558,-11.3606 6.738281,-15.13672 1.106716,-3.84105 1.660096,-8.5611 1.660156,-14.16016 -6e-5,-8.65874 -1.692765,-15.03894 -5.078124,-19.14062 -3.385468,-4.10144 -7.38937,-6.15222 -12.011719,-6.15235 -3.515664,1.3e-4 -6.4779,0.94414 -8.886719,2.83203 -1.822948,1.43242 -2.734405,2.92981 -2.734375,4.49219 -3e-5,1.04179 0.618459,2.66939 1.855469,4.88281 2.799444,5.01314 4.199182,9.27746 4.199219,12.79297 -3.7e-5,3.19021 -1.106807,5.85948 -3.320313,8.00782 -2.148468,2.14853 -4.850289,3.22274 -8.105469,3.22265 -3.645854,9e-5 -6.705747,-1.26944 -9.179687,-3.80859 -2.473972,-2.60407 -3.71095,-6.02204 -3.710938,-10.25391 -1.2e-5,-7.68218 3.320297,-14.42045 9.960938,-20.21484 6.640596,-5.79414 15.917931,-8.69127 27.832031,-8.69141 12.630147,1.4e-4 22.232997,3.02748 28.808597,9.08203 6.64054,5.98971 9.96085,13.24881 9.96093,21.77735 -8e-5,6.18499 -1.69279,11.91415 -5.07812,17.1875 -3.3855,5.20841 -9.92846,11.00268 -19.62891,17.38281 -6.510471,4.29694 -10.807342,8.13808 -12.890622,11.52344 -2.018278,3.32036 -3.255256,8.43104 -3.710937,15.33203 m -1.074219,14.25781 c 4.361928,3e-5 8.072862,1.52998 11.132812,4.58984 3.059836,3.05992 4.589786,6.77085 4.589846,11.13282 -6e-5,4.36198 -1.53001,8.10547 -4.589846,11.23047 -3.05995,3.05989 -6.770884,4.58984 -11.132812,4.58984 -4.362021,0 -8.072955,-1.52995 -11.132813,-4.58984 -3.059928,-3.125 -4.589874,-6.86849 -4.589843,-11.23047 -3.1e-5,-4.36197 1.529915,-8.0729 4.589843,-11.13282 3.059858,-3.05986 6.770792,-4.58981 11.132813,-4.58984"\r\n         id="path3775"\r\n         style="font-size:200px;font-weight:bold;fill:#ffffff;fill-opacity:1;font-family:Times New Roman;-inkscape-font-specification:Times New Roman" />\r\n    </g>\r\n  </g>\r\n</svg>\r\n' },
  url: '' }