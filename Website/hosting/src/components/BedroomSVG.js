import React from 'react';
import { svgs } from '../assets/remote/SVGs';
import { bedroomSVG } from '../assets/BedroomString';
import $ from 'jquery';
import { TimelineMax } from 'gsap';
import { withStyles } from 'material-ui/styles';
import { Paper } from 'material-ui'

const styles = theme => ({
  root: theme.mixins.gutters({
    paddingTop: 16,
    paddingBottom: 16,
    margin: theme.spacing.unit * 3,
  }),
});

var colorDefault = '#FFF';
var colorDuration = 1;

var tlShake = new TimelineMax({repeat:-1});

class BedroomSVG extends React.Component{

  constructor(props) {
		super(props);
    var bedroomDiv = document.createElement('div');
		bedroomDiv.innerHTML = bedroomSVG;
    this.svg = $(bedroomDiv).find('svg')[0];
    this.svg.setAttribute('preserveAspectRatio', 'xMidYMid meet');
    this.svg.setAttribute('viewBox', '0 0 1500 800')

    this.addButtonListeners = this.addButtonListeners.bind(this);

    this.hoverRemote = this.hoverRemote.bind(this);
    this.hoverPig = this.hoverPig.bind(this);
    this.hoverDiary = this.hoverDiary.bind(this);
    this.hoverSleep = this.hoverSleep.bind(this);
    this.hoverDresser = this.hoverDresser.bind(this);
    this.hoverDresser = this.hoverDresser.bind(this);
    this.hoverCactus = this.hoverCactus.bind(this);
    this.hoverFlower = this.hoverFlower.bind(this);

    this.leaveRemote = this.leaveRemote.bind(this);
    this.leavePig = this.leavePig.bind(this);
    this.leaveDiary = this.leaveDiary.bind(this);
    this.leaveSleep = this.leaveSleep.bind(this);
    this.leaveDresser = this.leaveDresser.bind(this);
    this.leaveDresser = this.leaveDresser.bind(this);
    this.leaveCactus = this.leaveCactus.bind(this);
    this.leaveFlower = this.leaveFlower.bind(this);

    this.clickRemote = this.clickRemote.bind(this);
    this.clickPig = this.clickPig.bind(this);
	};

  componentDidMount() {
    this.getElements();
    this.addButtonListeners();
  };

  getElements() {
    var buttonRemote = document.getElementById('buttonRemote');
    var buttonPig = document.getElementById('buttonPig');
    var buttonDiary = document.getElementById('buttonDiary');
    var buttonSleep = document.getElementById('buttonSleep');
    var buttonDresserTop = document.getElementById('buttonDresserTop');
    var buttonDresserBottom = document.getElementById('buttonDresserBottom');
    var buttonCactus = document.getElementById('buttonCactus');
    var buttonFlower = document.getElementById('buttonFlower');

    var remote = document.getElementById('remote');
    var remoteBottom = document.getElementById('remoteBottom');
    var remoteBody = document.getElementById('remoteBody');
    var remoteButtons = document.getElementById('remoteButtons');
    var remoteAntennaTop = document.getElementById('remoteAntennaTop');
    var remoteStickBottoms = document.getElementById('remoteStickBottoms');
    var remoteStickTops = document.getElementById('remoteStickTops');

    var pig = document.getElementById('pig');
    var pigBodyElements = document.getElementById('pigBodyElements');
    var pigSnoutFront = document.getElementById('pigSnoutFront');
    var pigBackElements = document.getElementById('pigBackElements');
    var pigEarInner = document.getElementById('pigEarInner');
    var pigNostrils = document.getElementById('pigNostrils');
    var pigEyes = document.getElementById('pigEyes');
    var pigCoinSlot = document.getElementById('pigCoinSlot');

    var diary = document.getElementById('diary')
    var diaryBottom = document.getElementById('diaryBottom');
    var diaryPages = document.getElementById('diaryPages');
    var diaryCover = document.getElementById('diaryCover');
    var diarySpine = document.getElementById('diarySpine');

    var sleep = document.getElementById('sleep');
    var sleepMount = document.getElementById('sleepMount');
    var sleepButton = document.getElementById('sleepButton');

    var dresser = document.getElementById('dresser');
    var dresserFrame = document.getElementById('dresserFrame');
    var dresserDrawers = document.getElementById('dresserDrawers');
    var dresserKnobs = document.getElementById('dresserKnobs');

    var cactus = document.getElementById('cactus');
    var cactusPotBody = document.getElementById('cactusPotBody');
    var cactusPotLip = document.getElementById('cactusPotLip');
    var cactusPotTop = document.getElementById('cactusPotTop');
    var cactusBody = document.getElementById('cactusBody');

    var flowers = document.getElementById('flowers');
    var flowerPotBottom = document.getElementById('flowerPotBottom');
    var flowerPotLip = document.getElementById('flowerPotLip');
    var flowerPotTop = document.getElementById('flowerPotTop');
    var leafBodyRight = document.getElementById('leafBodyRight');
    var leafBodyCenter = document.getElementById('leafBodyCenter');
    var leafBodyLeft = document.getElementById('leafBodyLeft');
    var flowerPetals = document.getElementById('flowerPetals');
    var flowerStamens = document.getElementById('flowerStamens');

    var car = document.getElementById('car');
    var carLights = document.getElementById('carLights');
    var carBody = document.getElementById('carBody');
    var carTires = document.getElementById('carTires');
    var carHubs = document.getElementById('carHubs');
    var carHandle = document.getElementById('carHandle');
    var carWindows = document.getElementById('carWindows');
    var carAntennaTop = document.getElementById('carAntennaTop');
    var carAnimation = document.getElementById('Animation');
    var carSmoke1 = document.getElementById('carSmoke1');
    var carSmoke2 = document.getElementById('carSmoke2');
    var carSmoke3 = document.getElementById('carSmoke3');
    var carShadow = document.getElementById('carShadow');

    var solarSystem = document.getElementById('solarSystem');
    var colorPlanets = document.getElementById('colorPlanets');

    var lampBottom = document.getElementById('lampBottom');
    var lampShade = document.getElementById('lampShade');
    var lampTop = document.getElementById('lampTop');
    var lampLight = document.getElementById('lampLight');

    var clock = document.getElementById('clock');
    var clockBody = document.getElementById('clockBody');
    var clockFace = document.getElementById('clockFace');
    var clockBellLeft = document.getElementById('clockBellLeft');
    var clockBellRight = document.getElementById('clockBellRight');
    var clockLegs = document.getElementById('clockLegs');
  };

    addButtonListeners() {
      buttonRemote.addEventListener('mouseover', this.hoverRemote);
      buttonPig.addEventListener('mouseover', this.hoverPig);
      buttonDiary.addEventListener('mouseover', this.hoverDiary);
      buttonSleep.addEventListener('mouseover', this.hoverSleep);
      buttonDresserTop.addEventListener('mouseover', this.hoverDresser);
      buttonDresserBottom.addEventListener('mouseover', this.hoverDresser);
      buttonCactus.addEventListener('mouseover', this.hoverCactus);
      buttonFlower.addEventListener('mouseover', this.hoverFlower);

      buttonRemote.addEventListener('mouseleave', this.leaveRemote);
      buttonPig.addEventListener('mouseleave', this.leavePig);
      buttonDiary.addEventListener('mouseleave', this.leaveDiary);
      buttonSleep.addEventListener('mouseleave', this.leaveSleep);
      buttonDresserTop.addEventListener('mouseleave', this.leaveDresser);
      buttonDresserBottom.addEventListener('mouseleave', this.leaveDresser);
      buttonCactus.addEventListener('mouseleave', this.leaveCactus);
      buttonFlower.addEventListener('mouseleave', this.leaveFlower);

      buttonRemote.addEventListener('click', this.clickRemote);
      buttonPig.addEventListener('click', this.clickPig);
  };

    removeButtonListeners() {
      buttonRemote.removeEventListener('mouseover', this.hoverRemote);
      buttonPig.removeEventListener('mouseover', this.hoverPig);
      buttonDiary.removeEventListener('mouseover', this.hoverDiary);
      buttonSleep.removeEventListener('mouseover', this.hoverSleep);
      buttonDresserTop.removeEventListener('mouseover', this.hoverDresser);
      buttonDresserBottom.removeEventListener('mouseover', this.hoverDresser);
      buttonCactus.removeEventListener('mouseover', this.hoverCactus);
      buttonFlower.removeEventListener('mouseover', this.hoverFlower);

      buttonRemote.removeEventListener('mouseleave', this.leaveRemote);
      buttonPig.removeEventListener('mouseleave', this.leavePig);
      buttonDiary.removeEventListener('mouseleave', this.leaveDiary);
      buttonSleep.removeEventListener('mouseleave', this.leaveSleep);
      buttonDresserTop.removeEventListener('mouseleave', this.leaveDresser);
      buttonDresserBottom.removeEventListener('mouseleave', this.leaveDresser);
      buttonCactus.removeEventListener('mouseleave', this.leaveCactus);
      buttonFlower.removeEventListener('mouseleave', this.leaveFlower);

      buttonRemote.removeEventListener('click', this.clickRemote);
      buttonPig.removeEventListener('click', this.clickPig);

      console.log("removed")
    }

  hoverShake(e) {
    tlShake.fromTo(e, 2, {x:-1}, {x:1, ease:RoughEase.ease.config({
      strength:8,
      points:20,
      template:Linear.easeNone,
      randomize:false
    }) , clearProps:"x"});
  };

  leaveShake() {
    tlShake.clear();
  };

  hoverRemote() {
    this.colorRemote();
    this.hoverShake(remote);
  };

  leaveRemote() {
    this.uncolorRemote();
    tlShake.clear();
  };

  clickRemote() {
    this.removeButtonListeners();
    this.colorRemote();
    tlShake.clear();

    let tlCar = new TimelineMax({onComplete: proxyFunction.bind(this)});
    function proxyFunction(): void {
      this.addButtonListeners();
      this.uncolorRemote();
    };
    TweenLite.to(carAnimation, colorDuration, {display: 'block'});
    TweenLite.to(carLights, colorDuration, {fill: '#F2B666'});
    TweenLite.to(carBody, colorDuration, {fill: '#C1272D'});
    TweenLite.to(carTires, colorDuration, {fill: '#333333'});
    TweenLite.to(carHubs, colorDuration, {fill: '#E6E6E6'});
    TweenLite.to(carHandle, colorDuration, {fill: '#B3B3B3'});
    TweenLite.to(carWindows, colorDuration, {fill: '#7FCAE5'});
    TweenLite.to(carAntennaTop, colorDuration, {fill: '#37474F'});
    tlCar.fromTo([carSmoke1, carSmoke2, carSmoke3], 7.5, {y:-0.5}, {y:0.5, ease:RoughEase.ease.config({strength:20, points:20, template:Linear.easeNone, randomize:false}) , clearProps:"x"});
    tlCar.to(car, 1.2, {x: -70, ease: Power2.easeout}, 0.7);
    tlCar.to(car, 1, {x: 40, y: 160, ease: Power2.easeout}, 2);
    tlCar.to(car, 0.6, {x: 200, ease: Power2.easeout}, 3.1);
    tlCar.to(car, 0.6, {y: 80, ease: Power2.easeout}, 3.8);
    tlCar.to(car, 0.7, {x: -30, y: -10, ease: Power2.easeout}, 4.5);
    tlCar.to(car, 0.8, {y: 0, ease: Power2.easeout}, 5.3);
    tlCar.to(car, 0.8, {x: 0, ease: Power2.easeout}, 6.2);
    tlCar.to(carAnimation, 0.1, {display: 'none'});
    tlCar.to([carLights, carBody, carTires, carHubs, carHandle, carWindows, carAntennaTop], colorDuration, {fill: colorDefault, delay: 0.2});
    console.log("click")
  };

  colorRemote() {
    TweenLite.to(remoteBottom, colorDuration, {fill: '#333333'});
    TweenLite.to(remoteBody, colorDuration, {fill: '#666666'});
    TweenLite.to(remoteButtons, colorDuration, {fill: '#CCCCCC'});
    TweenLite.to(remoteAntennaTop, colorDuration, {fill: '#000'});
    TweenLite.to(remoteStickBottoms, colorDuration, {fill: '#999999'});
    TweenLite.to(remoteStickTops, colorDuration, {fill: '#666666'});
  };

  uncolorRemote() {
    TweenLite.to(remoteBottom, colorDuration, {fill: colorDefault});
    TweenLite.to(remoteBody, colorDuration, {fill: colorDefault});
    TweenLite.to(remoteButtons, colorDuration, {fill: colorDefault});
    TweenLite.to(remoteAntennaTop, colorDuration, {fill: colorDefault});
    TweenLite.to(remoteStickBottoms, colorDuration, {fill: colorDefault});
    TweenLite.to(remoteStickTops, colorDuration, {fill: colorDefault});
  };

  hoverPig() {
    this.colorPig();
    this.hoverShake(pig);
  };

  leavePig() {
    this.uncolorPig();
    this.leaveShake();
  };

  clickPig() {
    this.removeButtonListeners();
    this.colorPig();
    tlShake.clear();
    this.clickTheft();

    let tlPig = new TimelineMax({onComplete: proxyFunction.bind(this)});
    function proxyFunction(): void {
      this.addButtonListeners();
      this.uncolorPig();
    };

    tlPig.to(pig, 2.3, {scaleX: 1.8, scaleY: 1.8, x:150, y: 325, yoyo: true, repeat: 1, ease: Sine.easeInOut}, 0.2)
  };

  colorPig() {
    TweenLite.to(pigBodyElements, colorDuration, {fill: '#EDC3D9'});
    TweenLite.to(pigSnoutFront, colorDuration, {fill: '#FCEDF7'});
    TweenLite.to(pigBackElements, colorDuration, {fill: '#B27D97'});
    TweenLite.to(pigEarInner, colorDuration, {fill: '#FCEDF7'});
    TweenLite.to(pigNostrils, colorDuration, {fill: '#37474F'});
    TweenLite.to(pigEyes, colorDuration, {fill: '#37474F'});
    TweenLite.to(pigCoinSlot, colorDuration, {fill: '#37474F'});
  };

  uncolorPig() {
    TweenLite.to(pigBodyElements, colorDuration, {fill: colorDefault});
    TweenLite.to(pigSnoutFront, colorDuration, {fill: colorDefault});
    TweenLite.to(pigBackElements, colorDuration, {fill: colorDefault});
    TweenLite.to(pigEarInner, colorDuration, {fill: colorDefault});
    TweenLite.to(pigNostrils, colorDuration, {fill: colorDefault});
    TweenLite.to(pigEyes, colorDuration, {fill: colorDefault});
    TweenLite.to(pigCoinSlot, colorDuration, {fill: colorDefault});
  };

  hoverDiary() {
    this.colorDiary();
    this.hoverShake(diary);
  };

  leaveDiary() {
    this.uncolorDiary();
    this.leaveShake();
  };

  colorDiary() {
    TweenLite.to(diaryBottom, colorDuration, {fill: '#4A99CE'});
    TweenLite.to(diaryPages, colorDuration, {fill: '#FFF'});
    TweenLite.to(diaryCover, colorDuration, {fill: '#ADE9F2'});
    TweenLite.to(diarySpine, colorDuration, {fill: '#4A99CE'});
  };

  uncolorDiary() {
    TweenLite.to(diaryBottom, colorDuration, {fill: colorDefault});
    TweenLite.to(diaryPages, colorDuration, {fill: colorDefault});
    TweenLite.to(diaryCover, colorDuration, {fill: colorDefault});
    TweenLite.to(diarySpine, colorDuration, {fill: colorDefault});
  };

  clickDiary() {

  };

  clickTheft() {
    const tlTheft = new TimelineMax();

    const startTime = 0.2;
    //color planets
    TweenLite.to(colorPlanets, colorDuration, {display: 'block'});
    //color lamp
    TweenLite.to(lampBottom, colorDuration, {fill: '#A7CAD3'});
    TweenLite.to(lampShade, colorDuration, {fill: '#FFF3C0'});
    TweenLite.to(lampTop, colorDuration, {fill: '#FCEA6B'});
    TweenLite.to(lampLight, colorDuration, {display: 'block'});

    //color alarm clock
    TweenLite.to(clockBody, colorDuration, {fill: '#148408'});
    TweenLite.to(clockFace, colorDuration, {fill: '#FFF'});
    TweenLite.to(clockBellLeft, colorDuration, {fill: '#D88D2B'});
    TweenLite.to(clockBellRight, colorDuration, {fill: '#D88D2B'});
    TweenLite.to(clockLegs, colorDuration, {fill: '#808080'});

    tlTheft.fromTo(lampTop, 0.2, {fill: '#BC0D0D'}, {fill: '#FCEA6B', repeat: 25}, startTime)
    tlTheft.fromTo(lampShade, 0.2, {fill: '#E26F6F'}, {fill: '#FFF3C0', repeat: 25}, startTime)
    tlTheft.fromTo(lampLight, 0.2, {fill: '#C98181'}, {fill: '#EAE1B2', repeat: 25}, startTime)

    tlTheft.fromTo(clock, 0.1, {y: 5, x: 3}, {
      y: -5,
      x: -3,
      yoyo: true,
      repeat: 50,
      ease: Sine.easeInOut
    }, startTime);

    tlTheft.fromTo(clockBellLeft, 0.2, {rotation: -4, transformOrigin: "50%", y: 3}, {
      rotation: 4,
      transformOrigin: "50%",
      y: -3,
      yoyo: true,
      repeat: 26,
      ease: Sine.easeInOut
    }, startTime);

    tlTheft.fromTo(clockBellRight, 0.2, {rotation: 4, transformOrigin: "50%", y: -3}, {
      rotation: -4,
      transformOrigin: "50%",
      y: 3,
      yoyo: true,
      repeat: 25,
      ease: Sine.easeInOut
    }, startTime);

    tlTheft.fromTo(solarSystem, 0.5, {rotation: 10, transformOrigin: "50%"}, {
      rotation: -10,
      yoyo: true,
      repeat: 10,
      ease: Sine.easeInOut
    }, startTime)

    tlTheft.to(solarSystem, 0.25, {rotation: 0, transformOrigin: "50%"}, startTime+5)
    tlTheft.to([colorPlanets, lampLight], 0.1, {display: 'none'}, startTime+5.3)
    tlTheft.to([clockBody, clockFace, clockBellLeft, clockBellRight, clockLegs, lampBottom, lampShade, lampTop], colorDuration, {fill: colorDefault, delay: 0.2});
  }

  hoverSleep() {
    this.colorSleep()
    this.hoverShake(sleep);
  };

  leaveSleep() {
    this.uncolorSleep();
    this.leaveShake();
  };

  colorSleep() {
    TweenLite.to(sleepMount, colorDuration, {fill: '#C6C6C6'});
    TweenLite.to(sleepButton, colorDuration, {fill: '#AA1721'});
  };

  uncolorSleep() {
    TweenLite.to(sleepMount, colorDuration, {fill: colorDefault});
    TweenLite.to(sleepButton, colorDuration, {fill: colorDefault});
  };

  hoverDresser() {
    this.colorDresser();
    this.hoverShake(dresser);
  };

  leaveDresser() {
    this.uncolorDresser();
    this.leaveShake();
  };

  colorDresser() {
    TweenLite.to(dresserFrame, colorDuration, {fill: '#896003'});
    TweenLite.to(dresserDrawers, colorDuration, {fill: '#A87420'});
    TweenLite.to(dresserKnobs, colorDuration, {fill: '#896003'});
  };

  uncolorDresser() {
    TweenLite.to(dresserFrame, colorDuration, {fill: colorDefault});
    TweenLite.to(dresserDrawers, colorDuration, {fill: colorDefault});
    TweenLite.to(dresserKnobs, colorDuration, {fill: colorDefault});
  };

  hoverCactus() {
    this.colorCactus();
    this.hoverShake(cactus);
  };

  leaveCactus() {
    this.uncolorCactus();
    this.leaveShake();
  };

  colorCactus() {
    TweenLite.to(cactusPotBody, colorDuration, {fill: '#A55447'});
    TweenLite.to(cactusPotLip, colorDuration, {fill: '#965040'});
    TweenLite.to(cactusPotTop, colorDuration, {fill: '#603813'});
    TweenLite.to(cactusBody, colorDuration, {fill: '#8cc63f'});
  };

  uncolorCactus() {
    TweenLite.to(cactusPotBody, colorDuration, {fill: colorDefault});
    TweenLite.to(cactusPotLip, colorDuration, {fill: colorDefault});
    TweenLite.to(cactusPotTop, colorDuration, {fill: colorDefault});
    TweenLite.to(cactusBody, colorDuration, {fill: colorDefault});
  };

  hoverFlower() {
    this.colorFlower();
    this.hoverShake(flowers);
  };

  leaveFlower() {
    this.uncolorFlower();
    this.leaveShake();
  };

  colorFlower() {
    TweenLite.to(flowerPotBottom, colorDuration, {fill: '#A55447'});
    TweenLite.to(flowerPotLip, colorDuration, {fill: '#965040'});
    TweenLite.to(flowerPotTop, colorDuration, {fill: '#603813'});
    TweenLite.to(leafBodyRight, colorDuration, {fill: '#39B54A'});
    TweenLite.to(leafBodyCenter, colorDuration, {fill: '#006837'});
    TweenLite.to(leafBodyLeft, colorDuration, {fill: '#009245'});
    TweenLite.to(flowerPetals, colorDuration, {fill: '#F9D7E4'});
    TweenLite.to(flowerStamens, colorDuration, {fill: '#FBB03B'});
  };

  uncolorFlower() {
    TweenLite.to(flowerPotBottom, colorDuration, {fill: colorDefault});
    TweenLite.to(flowerPotLip, colorDuration, {fill: colorDefault});
    TweenLite.to(flowerPotTop, colorDuration, {fill: colorDefault});
    TweenLite.to(leafBodyRight, colorDuration, {fill: colorDefault});
    TweenLite.to(leafBodyCenter, colorDuration, {fill: colorDefault});
    TweenLite.to(leafBodyLeft, colorDuration, {fill: colorDefault});
    TweenLite.to(flowerPetals, colorDuration, {fill: colorDefault});
    TweenLite.to(flowerStamens, colorDuration, {fill: colorDefault});
  };

  render() {
    const { classes } = this.props;

    const inputs = {
			svg: {
				dangerouslySetInnerHTML: {__html: $(this.svg).prop('outerHTML')},
        overflow: 'hidden',
			},
    };

    return (
      <Paper className={classes.root} elevation={3}>
        <div {...inputs.svg}/>
      </Paper>
    )
  };
};

export default withStyles(styles)(BedroomSVG)
