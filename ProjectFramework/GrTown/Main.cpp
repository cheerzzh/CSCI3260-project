/// GraphicsTown2006
// written by Michael   - originally written Fall, 2000
//
// updated Fall 2006 - more testing code
// updated Fall 2008 - switched some internal stuff (no more linked lists)

#include "GrTown_PCH.H"
#include "GraphicsTownUI.H"
#include "FlyCamera.H"
#include "GrWorld.H"
#include "DrawUtils.H"
#include "Examples/Objects.H"
#include "Examples/SimpleBehaviors.H"
#include "Examples/Cars.H"

// for setting up shader paths and textures
#include "Utilities/ShaderTools.H"
#include "Utilities/Texture.H"


// Example code (objects and behaviors)
#include "Examples/Suburbs.H"

#include "Roads/Roads.H"
#include "Roads/Drive.H"


// define this to get 2 cars that always turn
// #define TESTCARS

const int numCars = 1;
const int nGrids = 5;

int main(int /*argc*/, char** /*argv*/)
{ 

  // put in some texture paths - look all over the place
	texturePaths.push_back(".");
	texturePaths.push_back("..");
	texturePaths.push_back("Textures/signs");
	texturePaths.push_back("../Textures/signs");
	texturePaths.push_back("Textures/Textures");
	texturePaths.push_back("../Textures/Textures");
	texturePaths.push_back("Textures/Objects");
	texturePaths.push_back("../Textures/Objects");

	shaderPaths.push_back("Shaders");
	shaderPaths.push_back("../Shaders");


  // add some more stuff
  //GrObject* o1 = new Church;
  //o1->interesting = true;
  //o1->laX = 0; o1->laY = 0; o1->laZ = 0;
  //o1->lfX = -50; o1->lfY = 100; o1->lfZ = 300;
  //add(o1,-100,0,100,pi/2.f);

  GrObject* church1 = new LutheranChurch;
  church1->interesting = true;
  church1->name = "St.Olav Church";
  church1->laX = 0; church1->laY = 0; church1->laZ = 0;
  church1->lfX = -50; church1->lfY = 100; church1->lfZ = 250;
  add(church1, -50, 0, 100, pi / 2.f);
  
  GrObject* church2 = new OrthodoxChurch;
  church2->interesting = true;
  church2->name = "Alexander Nevsky Church";
  church2->laX = 0; church2->laY = 0; church2->laZ = 0;
  church2->lfX = -50; church2->lfY = 100; church2->lfZ = 250;
  add(church2, -50, 0, 200, pi / 2.f);

  GrObject* gate = new CityGate;
  gate->interesting = true;
  gate->name = "Viru Gate";
  gate->laX = 0; gate->laY = 0; gate->laZ = 0;
  gate->lfX = -50; gate->lfY = 100; gate->lfZ = 300;
  add(gate, -16, 0, 400, 0);

  GrObject* tvtower = new TVTower;
  tvtower->name = "TV Tower";
  tvtower->interesting = true;
  tvtower->laX = 0; tvtower->laY = 0; tvtower->laZ = 0;
  tvtower->lfX = -50; tvtower->lfY = 100; tvtower->lfZ = 300;
  add(tvtower, -100, 0, 300, pi / 2.f);

  GrObject* market = new IndoorMarket;
  market->name = "Central Market";
  market->interesting = true;
  market->laX = 0; market->laY = 0; market->laZ = 0;
  market->lfX = -50; market->lfY = 100; market->lfZ = 300;
  add(market, -50, 0, 450, pi / 2.f);

  GrObject* townhall = new TownHall;
  townhall->name = "Town Hall";
  townhall->interesting = true;
  townhall->laX = 0; townhall->laY = 0; townhall->laZ = 0;
  townhall->lfX = -50; townhall->lfY = 100; townhall->lfZ = 300;
  add(townhall, -50, 0, 620, pi / 2.f);

  // *****************************************************************
  //  Make your town here

  // add snowman 
  GrObject* snowman1 = new SnowMan(-100, 0, -50, 30);
  add(snowman1);
  snowman1->name = "snowman1";
  // make it an interesting object (so its easy to look at)
  // and give a place to look at it from
  // this is a good object to look at since it won't move //bookmark
  snowman1->interesting = true;
  snowman1->laX = 0; snowman1->laY = 0; snowman1->laZ = 0;
  snowman1->lfX = 70; snowman1->lfY = 50; snowman1->lfZ = 0;

  // first, some really simple things

  // cubes are particularly easy since they set their own position
  // we need to raise the cube since we're giving the position of the
  // center 
  GrObject* cube1 = new ShadedCube(-50,5,-50,10,   .7f, .6f, .3f);
  add(cube1);
  cube1->name = "Cube1";
  // make it an interesting object (so its easy to look at)
  // and give a place to look at it from
  // this is a good object to look at since it won't move //bookmark
  cube1->interesting = true;
  cube1->laX = 0; cube1->laY = 0; cube1->laZ = 0;
  cube1->lfX = 70; cube1->lfY = 50; cube1->lfZ = 0;

  // make another cube - make this one spin around
  GrObject* cube2 = new Cube(-50,5,-25,10,   .3f, .6f, .7f);
  add(cube2);
  new Spin(cube2);

  // now to demonstrate hierarchy - make another cube that spins, and
  // put a smaller cube on top of it
  GrObject* cube3 = new Cube(-50,5,-75,10,   .7f, .3f, .6f);
  add(cube3);
  new Spin(cube3,.001f);
  GrObject* cube4 = new Cube(1,7.5,0, 2, 1,0,1);
  cube3->add(cube4);

  // test car
  Car* c1 = new HatchBack(1);
  cube4->add(c1);
  



    ////////////////////////////////////////////////////////////////////////
  // now to make a real town!
  int r,c;
  // make a 5x5 grid of town blocks - 5 houses per
  for( r=0; r<5; r++) {
	  for( c=0; c<5; c++) {
		  add(new SimpleSubdivision(5),static_cast<float>(r*530),0,static_cast<float>(c*230));
		  add(new StraightRoad(static_cast<float>(r*530),static_cast<float>(c*230),static_cast<float>(r*530+500),static_cast<float>(c*230)));
	  }
  }
  //// make cross streets
  for(int r=0; r<=5; r++) {
	  for(c=0; c<4; c++) {
		  add(new StraightRoad(static_cast<float>(r*530 - 15), static_cast<float>(c*230 + 15), static_cast<float>(r*530 - 15), static_cast<float>(c*230+215)));
	  }
  }

  //// make intersections
  //// make an intersection intersesting so we can look at it
  for(int r=0; r<=5; r++) {
	  for(c=0; c<5; c++) {
		  GrObject* g = new Intersection(static_cast<float>(r*530-15), static_cast<float>(c*230));
		  if ( (r==2) && (c==3) ) {
			  g->interesting = 1;
			  g->name = "Intersection(2,3)";
			  g->laX = static_cast<float>(r*530-15);    g->laY = 0;    g->laZ = static_cast<float>(c*230);
			  g->lfX = static_cast<float>(r*530+25);   g->lfY = 100;   g->lfZ = static_cast<float>(c*230+150);
		  }
		  add(g);
	  }
  }

#ifndef TESTCARS
  // add some cars
  for(int r=0; r<50; r++) {
	Car* c;
	switch(rand() % 3) {
	  case 0: c = new Van(rand()); break;
	  case 1: c = new SUV(rand()); break;
	  case 2: c = new HatchBack(rand()); break;
	}
	add(c);
    new RandomDrive(c,theRoads[rand() % theRoads.size()],.2f,rand() % 2);
  }
#endif

#ifdef TESTCARS
  // two cars - one always turns right, one always turns left
  Car* c1 = new HatchBack(1);
  Car* c2 = new HatchBack(2);
	add(c1);
	add(c2);
  new RandomDrive(c1,theRoads[7],.5,0,-1);
  new RandomDrive(c2,theRoads[8],.5,0,1);

#endif

// ========== TODO1: ===================
// The following code added a round race track, 
//  you're required to replace it with an elliptical race track and put two cars running on it.


	// a race track
    Road* t = new RoundRoad(-250,250,100);
	add(t);
	t->name = "Track";
	t->interesting = true;
	t->laX = -250; t->laY = 0;   t->laZ = 250;
	t->lfX = -600; t->lfY = 200; t->lfZ = 450;
	// make cars go around the track
	Car* h = new HatchBack(1);
	h->name="Race1";		// warning! we can only do this since we don't delete
	add(h);
	new SimpleDrive(h,t,0,0);
	h = new HatchBack(3);
	h->name="Race2";		// warning! we can only do this since we don't delete
	add(h);
	Drive* d = new SimpleDrive(h,t,0,1);
	d->speed *= 2;


  // *****************************************************************
  // now make a UI
  FlyCamera* fc = new FlyCamera;
  Map* m = new Map;

  add(fc);
  add(m);

  GraphicsTownUI grTown;
  grTown.make_window();
  grTown.townView->defaultCamera = fc;
  grTown.window->show();

  Fl::run();
  return 1;
}

