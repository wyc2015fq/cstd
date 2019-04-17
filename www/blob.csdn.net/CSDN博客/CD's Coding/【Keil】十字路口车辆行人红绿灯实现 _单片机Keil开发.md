# 【Keil】十字路口车辆行人红绿灯实现 _单片机Keil开发 - CD's Coding - CSDN博客





2015年05月04日 19:06:00[糖果天王](https://me.csdn.net/okcd00)阅读数：3566








## 前言

这次作为嵌入式系统与应用的中期考试，老师要求写一个红绿灯的程序（包括车辆&行人），要求在单片机上可持续合理化运行

简化如下：十字路口中，南北方向与东西方向均只考虑直行，行人红绿灯仅考虑东侧的南北向人行道

话说……实验要求什么的看起来似乎很长哎…… 如果不想看那么长的实验要求的话，推荐用书签直接跳转到想看的地方去~




## 实验要求文档



PREPARATION

You will need aLaunchPad, a three switches, three 10kΩ resistors, six LEDs, and six 470Ωresistors. Run the Lab10 starter file in the simulator and on the realboard just to make sure the configurations are correct.
 Download the data sheetfor the LED [**HLMP-4700.pdf**](https://courses.edx.org/c4x/UTAustinX/UT.6.01x/asset/HLMP-4700.pdf)

STARTER PROJECT

Lab10_TrafficLight

PURPOSE

This lab hasthese major objectives: 1) the understanding and implementing of indexed datastructures; 2) learning how to create a segmented software system; and 3) thestudy of real-time synchronization by designing
 a finite state machinecontroller. Software skills you will learn include advanced indexed addressing,linked data structures, creating fixed-time delays using the SysTick timer, anddebugging real-time systems. Please read the entire lab before starting.

SYSTEM REQUIREMENTS

Consider a4-corner intersection as shown in Figure 10.1. There are two one-way streetsare labeled South (cars travel South) and West (cars travel West). There arethree inputs to your LaunchPad, two are car sensors,
 and one is a pedestriansensor. The South car sensor will be true (3.3V) if one or more cars are nearthe intersection on the South road. Similarly, the West car sensor will be true(3.3V) if one or more cars are near the intersection on the West road. The Walksensor
 will be true (3.3V) if a pedestrian is present and he or she wishes tocross in any direction. This walk sensor is different from a walk button onmost real intersections. This means when you are testing the system, you mustpush and hold the walk sensor until
 the FSM recognizes the presence of thepedestrian. Similarly, you will have to push and hold the car sensor until theFSM recognizes the presence of the car. In this simple system, if the walksensor is +3.3V, there is pedestrian to service, and if the walk sensor
 is 0V,there are no people who wish to walk. In a similar fashion, when a car sensoris 0V, it means no cars are waiting to enter the intersection. You willinterface 6 LEDs that represent the two Red-Yellow-Green traffic lights, andyou will use the PF3 green
 LED for the “walk” light and the PF1 red LED for the“don’t walk” light. When the “walk” condition is signified, pedestrians areallowed to cross. When the “don’t walk” light flashes (and the two trafficsignals are red), pedestrians should hurry up and finish
 crossing. When the“don’t walk” condition is on steady, pedestrians should not enter theintersection.

*Figure 10.1.Traffic Light Intersection.*

Traffic shouldnot be allowed to crash. I.e., there should not be only a green or only ayellow LED on one road at the same time there is only a green or only a yellowLED on the other road. You should exercise common
 sense when assigning thelength of time that the traffic light will spend in each state; so that thegrading engine can complete the testing in a reasonable amount of time. Eachtraffic light pattern must be on for at least ½ second but for at most 5seconds.
 (During simulation grading, we suggest you make all times less than 1second.) Cars should not be allowed to hit the pedestrians. The walk sequenceshould be realistic, showing three separate conditions: 1) “walk”, 2) “hurryup” using a flashing LED, and 3) “don’t
 walk”. You may assume the three sensorsremain active for as long as service is required. The “hurry up” flashingshould occur at least twice but at most 4 times.

The **automaticgrader** can only check for function (does a pattern of inputs, resultin the correct outputs.) In particular, the grader performs these checks:

0) At all times,there should be exactly one of the {red, yellow, green} traffic lights activeon the south road. At all times, there should be exactly one of the {red,yellow, green} traffic lights active on the west
 road. To switch a light fromgreen to red it must be yellow for at least ½ sec. The grader checks for theproper sequence of output patterns but does not measure the time the FSM spendsin each state. The “walk” and “don’t walk” lights should never both be on
 atthe same time.

1) Do not allowcars to crash into each other. This means there can never be a green or yellowon one road at the same time as a green or yellow on the other road. Engineersdo not want people to get hurt.

2) Do not allowpedestrians to walk while any cars are allowed to go. This means there cannever be a green or yellow on either road at the same time as a “walk” light.Furthermore, there can never be a green or yellow
 on either road at the sametime as the “don’t walk” light is flashing. If a green light is active on oneof the roads, the “don’t walk” should be solid red. Engineers do not wantpeople to get hurt.

3) If just thesouth sensor is active (no walk and no west sensor), the lights should adjustso the south has a green light within 5 seconds (I know this value isunrealistically short, but it makes the grading faster).
 The south light shouldstay green for as long as just the south sensor is active.

4) If just thewest sensor is active (no walk and no south sensor), the lights should adjustso the west has a green light within 5 seconds. The west light should staygreen for as long as just the west sensor is active.

5) If just thewalk sensor is active (no west and no south sensor), the lights should adjustso the “walk” light is green within 5 seconds. The “walk” light should staygreen for as long as just the walk sensor is active.

6) If all threesensors are active, the lights should go into a pattern such within one20-second period that the west light is green for at least 1 second, the southlight is green for at least 1 second and the “walk”
 light is green for at least1 second.

The gradingengine can only check for function, not for the quality of your software. Thissection describes, in qualitative manner, what we think is good design. Thereis no single, “best” way to implement your system.
 A “good” solution will haveabout 9 to 30 states in the finite state machine, and provides for inputdependence. Try not to focus on the civil engineering issues. I.e., first builda quality computer engineering solution that is easy to understand and easy tochange,
 and then adjust the state graph so it passes the functional tests ofthe automatic grader. Because we have three inputs, there will be 8 next statelinks. One way to draw the FSM graph to make it easier to read is to use X tosignify don’t care. For example,
 compare the two FSM graphs in Figure 10.2.Drawing two arrows labeled **01** and **11** is thesame as drawing one arrow with the label **X1**. When we implement thedata structure, however, we will expand the shorthand
 and explicitly list allpossible next states.

*Figure 10.2. FSMfrom Chapter 10 redrawn with a short hand format.*

The followingare some qualitative requirements, which we think are important, but for whichthe **automatic grader** may or may not be able to evaluate.

0) The systemprovides for input dependence. This means each state has 8 arrows such that thenext state depends on the current state and the input. This means you cannotsolve the problem by simply cycling through all
 the states regardless of theinput.

1) Because wethink being in a state is defined by the output pattern, we think you shouldimplement a Moore and not a Mealy machine. However, your scheme should use alinked data structure stored in ROM.

2) There shouldbe a 1-1 mapping between FSM graph and data structure. For a Moore machine,this means each state in the graph has a name, an output, a time to wait, and 8next state links (one for each input). The data
 structure has exactly thesecomponents: a name, an output, a time to wait, and 8 next state pointers (onefor each input). There is no more or no less information in the data structurethen the information in the state graph.

3) There can beno conditional branches in program, other than the **while** in **SysTick_Wait** andthe **for** in**SysTick_Wait10ms**.
 This willsimplify debugging make the FSM engine trivial.

4) The stategraph defines exactly what the system does in a clear and unambiguous fashion.In other words, do not embed functionality (e.g., flash 3 times) into thesoftware that is not explicitly defined in the state
 graph.

5) Each statehas the same format of each state. This means every state has exact one name,one 8-bit output (could be stored as one or two fields in the struct), one timeto wait, and 8 next indices.

6) Please usegood names and labels (easy to understand and easy to change). Examples of badstate names are **S0** and **S1**.

7) There shouldbe 9 to 30 states with a Moore finite state machine. If your machine has morethan 30 states, you have made it more complicated than we had in mind. Usuallystudents with less than 9 states did not flash
 the “don’t walk” light, or theyflashed the lights using a counter.  Counters and variables violatethe “no conditional branch” requirement.

In real productsthat we market to consumers, we put the executable instructions and the finitestate machine linked data structure into the nonvolatile memory such as flashEEPROM. A good implementation will allow minor
 changes to the finite machine(adding states, modifying times, removing states, moving transition arrows,changing the initial state) simply by changing the linked data structure,without changing the executable instructions. Making changes to executable coderequires
 you to debug/verify the system again. If there is a 1-1 mapping fromFSM to linked data structure, then if we just change the state graph and followthe 1-1 mapping, we can be confident our new system operate the new FSMproperly. Obviously, if we add another
 input sensor or output light, it may benecessary to update the executable part of the software, re-assemble and retestthe system.

The grader willactivate the PLL so the system runs at 80 MHz, you must not modify this rate.

There aremany **civil engineering questions** that students ask. How youchoose to answer these questions will determine how good a civil engineer youare but should not affect your grade on this lab. For
 each question, there aremany possible answers, and you are free to choose how you want to answer it.

0) How longshould I wait in each state?  *Possible answer*: traffic lightsat 1 to 2 seconds of real people time.*Flashing “don’t walk”* on for½ sec, off for a ½ sec and repeat 3 times.

1) What happensif I push 2 or 3 buttons at a time? *Required operation*: cyclethrough the requests servicing them in a round robin fashion.

2) What if Ipush the walk button, but release it before the light turns to walk? *Possibleanswer*: ignore the request as if it never happened. *Possible answer*:service it or ignore it depending on exactly
 when it occurred.

3) What if Ipush a car button, but release it before it is serviced? *Possibleanswer*: ignore the request as if it never happened (e.g., car came to a redlight, came to a full stop, and then made a legal turn).  *Possibleanswer*:
 service the request or ignore it depending on when it occurred.

4) Assume thereare no cars and the light is green on the North, what if a car now comes on theEast? Do I have to recognize a new input right away or wait until the end ofthe wait time? *Possible answer:* no,
 just wait until the end ofthe current wait, then service it. *Possible answer*: yes; breakstates with long waits into multiple states with same output but shorter waits.

5) What if thewalk button is pushed while the don’t walk light is flashing? *Possibleanswer*: ignore it, go to a green light state and if the walk button isstill pushed, then go to walk state again. *Possible answer:* ifno cars are waiting, go back to the walk state. *Possible answer*:remember that the button was pushed, and go to a walk state after the nextgreen light state.

6) Does the walkoccur on just one street or both? *Required operation*: stop all carsand let people walk across either or both streets. A green (or yellow) light inany direction while the “walk” light is on
 will cause the automatic grader topenalize you for failing check #2. The pedestrian sensor does not know whichstreet the pedestrian(s) want to cross, so you must direct all cars to stopwhile pedestrians may be in the road. You are not allowed to add additionalpedestrian
 sensor because the automatic grader is built to handle only theconfiguration shown in Figure 10.1.



Let's walkthrough the steps of building a **state transition table**.  Astate transition table has exactly the same information as the state transitiongraph, but in tabular form. Let's begin with the
 format of the table. Therewill be a column for the state number; we will number the states sequentiallyfrom 0. We will skip the state name for now, and come back to it later. Thenext two columns will define the output patterns for six traffic lights and twowalk
 lights. In this table we specified the output for the 6 LEDs as "WestGreen", but once we translate it to software we will replace "WestGreen" with the explicit value to output, e.g., 0x0C . Often we will needto have multiple states with the same output value,
 but for now we will simplyadd a state for each possible output pattern, and later we will add additionrows as needed. The next column is the time to wait with this output. The lasteight columns will be the next states for each possible input pattern. Recallthe
 FSM controller will 1) output, 2) wait, 3)  input, and 4) change tothe next state depending on the current state and the input. So notice thecolumns of this table are given in this 1,2,4 order.


|**Num**|**Name**|**6-LED**|**PF3,PF1**|**Time**|**In=0**|**In=1**|**In=2**|**In=3**|**In=4**|**In=5**|**In=6**|**In=7**|
|----|----|----|----|----|----|----|----|----|----|----|----|----|
|0|GoWest|West green|Red||||||||||
|1||West yellow|Red||||||||||
|2||South green|Red||||||||||
|3||South yellow|Red||||||||||
|4||All red|Green||||||||||
|5||All red|Red||||||||||
|6||All red|off||||||||||

Each stateshould have a descriptive name. These names will be labels in the C code, souse no spaces or special characters. The name should be short. Most importantlythe state name should describe *the state of the system, or what youbelieve to the true*. For example, **GoWest** could mean carscan go on the west road. Make the times short for this lab, neither graderchecks the times. It can be short (100ms) in simulation and 500ms on the realboard.
 Next, fill what to do for each input. The **in=0** field iseasy (0 means no cars or people). For example, what should you do if the westroad is green and there are no cars or people?  Another easy one is** in=7** (busywith cars and
 people). For **in=7**, we need to cycle through west,south and walk: {west green, west yellow, south green, south, yellow, walk,don't walk, walk off, don't walk, walk off}. To make the don't walk light flashtwice, we will need to add two more
 states 7,8 (rows in the table). State 7will have the road outputs red and the don't walk.  State 8  willhave the road outputs red and the walk LEDs off. In summary, the **in=7** inputpattern will cycle through states 0,1,2,3,4,5,6,7,8.

Once you havethis table, you convert it to C code. This is the structure from Program 10.4

**struct State {  unsigned long Out;   unsigned long Time;  unsigned long Next[4];};**

Modify thisstructure adding a line for Outputs to PF3, PF1. Change the 4 to 8, becausewith 3 inputs, each state needs 8 possible next states.  Convert the abovestate table into the FSM structure. The engine will execute 

1) Output 6-LEDs

2) Output PF3, PF1

3) Wait time

4) Input from sensors

5) Go to next state

This C code will be similar to the Traffic FSM example in the Program 10.4





**#define SENSOR  (*((volatile unsigned long *)0x4002400C))#define LIGHT   (*((volatile unsigned long *)0x400050FC))// Linked data structurestruct State {  unsigned long Out;   unsigned long Time;    unsigned long Next[4];}; typedef const struct State STyp;#define goN   0#define waitN 1#define goE   2#define waitE 3STyp FSM[4]={ {0x21,3000,{goN,waitN,goN,waitN}},  {0x22, 500,{goE,goE,goE,goE}}, {0x0C,3000,{goE,goE,waitE,waitE}}, {0x14, 500,{goN,goN,goN,goN}}};unsigned long S;  // index to the current state unsigned long Input; int main(void){ volatile unsigned long delay;  PLL_Init();       // 80 MHz, Program 10.1  SysTick_Init();   // Program 10.2  SYSCTL_RCGC2_R |= 0x12;      // 1) B E  delay = SYSCTL_RCGC2_R;      // 2) no need tounlock  GPIO_PORTE_AMSEL_R &= ~0x03; // 3) disable analog function on PE1-0  GPIO_PORTE_PCTL_R &=~0x000000FF; // 4) enable regular GPIO  GPIO_PORTE_DIR_R &= ~0x03;   // 5) inputs on PE1-0  GPIO_PORTE_AFSEL_R &=~0x03; // 6) regular function on PE1-0  GPIO_PORTE_DEN_R |= 0x03;    // 7) enable digital onPE1-0  GPIO_PORTB_AMSEL_R &= ~0x3F; // 3) disable analog function on PB5-0  GPIO_PORTB_PCTL_R &= ~0x00FFFFFF; // 4) enable regular GPIO  GPIO_PORTB_DIR_R |= 0x3F;    // 5) outputs on PB5-0  GPIO_PORTB_AFSEL_R &= ~0x3F; // 6) regular function on PB5-0  GPIO_PORTB_DEN_R |= 0x3F;    // 7) enable digital onPB5-0  S = goN;    while(1){    LIGHT = FSM[S].Out;  // set lights    SysTick_Wait10ms(FSM[S].Time);    Input = SENSOR;     // read sensors    S = FSM[S].Next[Input];    }}***Program 10.4. Linked data structureimplementation of the traffic light controller (C10_TableTrafficLight)*

## PROCEDURE

R9, R10, R25, and R29 are zero-ohmresistors on the LaunchPad. Notice in Figure 10.3 that R9 shorts PD0 to PB6,R10 shorts PD1 to PB7, R25 connects PB0 to the USB device header, and R29connects PB1 to +5V. If you use PB1 or PB0,
 make sure R26 and R29 are removed.If you use both PD0 and PB6, remove R9. If you use both PD1 and PB7, removeR10.  R25 and R29 are not on the older LM4F120 LaunchPads, just the newTM4C123 LaunchPads. The TM4C123 LaunchPads I bought over summer 2013 did nothave
 R25 and R29 soldered on, so I just had to worry about R9 and R10.

*Figure 10.3. Switch and LED interfaces on the Tiva LaunchPadEvaluation Board. The zero ohm resistors can be removed so the correspondingpin can be used for its regular purpose.*

We posted a version 1.0.0.6 which is littlemore reasonable about what it passes and what it fails. As with all graders, itbegins by checking initialization registers. During the I/O portion of thegrading, we get a notification
 whenever you write to either of the outputports. The grader checks for valid output pattern sequences. We have defined9valid output patterns, listed below. For each valid output pattern, there area only finite number of valid output patterns that could be
 next.

Pattern 1) **All lights are red.** Once the output is at this pattern,the valid next patterns are {1,2,4,6,7,8,9} 

Pattern 2) **West is green**,south is red, don’t walk is red**.** Oncethe output is at this pattern, the valid next patterns are {2,3} 

Pattern 3) **West is yellow**,south is red, don’t walk is red**.** Oncethe output is at this pattern, the valid next patterns are {1,3,4,6} 

Pattern 4) **South is green**,west is red, don’t walk is red**.** Oncethe output is at this pattern, the valid next patterns are {4,5} 

Pattern 5) **South isyellow**, west is red, don’t walk is red**.** Once the output is at this pattern,the valid next patterns are {1,2,5,6} 

Pattern 6) **Walk is green**,south is red, west is red**.** Oncethe output is at this pattern, the valid next patterns are {1,6,7} 

Pattern 7) **Don’t Walk isoff**, south is red, west is red**.** Once the output is at this pattern,the valid next patterns are {1,2,4,6,7,8,9}

Pattern 8) **Don’t Walk isoff**, west is red,** south isgreen.** Once theoutput is at this pattern, the valid next patterns are {4,5} 

Pattern 9) **Don’t Walk isoff**, **west is green**,south is red**.** Once theoutput is at this pattern, the valid next patterns are {2,3}

There are a couple of consequences of thisgrading algorithm:

1) You should output to the road lights first and then to the walk lights,

2) You should output to the walk and don't walk lights at the same time

3) For simulation, we do not check for timing, so make the delays short duringsimulation testing.

Most microcontrollershave a rich set of timer functions. For this lab, you will the PLL and SysTicktimer to wait a prescribed amount of time.

The basic approach tothis lab will be to first develop and debug your system using the simulator.After the software is debugged, you will interface actual lights and switchesto the LaunchPad and run your software on the real microcontroller.
 As you haveexperienced, the simulator requires different amount of actual time as comparedto simulated time. On the other hand, the correct simulation time is maintainedin the SysTick timer, which is decremented every cycle of simulation time. Thesimulator
 speed depends on the amount of information it needs to update intothe windows.   Because we do not want to wait the minutes required foran actual intersection, the cars in this traffic intersection travel muchfaster than “real” cars.  In other words, you are
 encouraged to adjust thetime delays so that the operation of your machine is convenient for you todebug and for the grading engine to observe during demonstration.

**Part a)**Decide which port pins you will use for the inputs and outputs.Avoid the pins with hardware already connected. Run the starter code
 in thesimulator to see which ports are available for the lights and switches; thesechoices are listed in Tables 10.1 and 10.2. The “don’t walk” and “walk” lightsmust be PF1 and PF3 respectively, but where to attach the others have someflexibility. In particular,
 Table 10.1 shows you three possibilities for howyou can connect the six LEDs that form the traffic lights.Table 10.2 shows youthree possibilities for how you can connect the three positive logic switchesthat constitute the input sensors. Obviously, you will
 not connect both inputsand outputs to the same pin.


|Red east/west|PA7|PB5|PE5|
|----|----|----|----|
|Yellow east/west|PA6|PB4|PE4|
|Green east/west|PA5|PB3|PE3|
|Red north/south|PA4|PB2|PE2|
|Yellow north/south|PA3|PB1|PE1|
|Green north/south|PA2|PB0|PE0|

*Table 10.1. Possible ports to interface the traffic lights(yellow is suggested or default).*


|Walk sensor|PA4|PB2|PE2|
|----|----|----|----|
|North/south sensor|PA3|PB1|PE1|
|East/west sensor|PA2|PB0|PE0|

*Table 10.2. Possible ports to interface the sensors (yellow issuggested or default).*



**Part b)**Design a finite state machine that implements a good trafficlight system. Draw a graphical picture of your finite state machine showing
 thevarious states, inputs, outputs, wait times and transitions.

**Part c)**Write and debug the C code that implements the traffic lightcontrol system. During the debugging phase with the simulator, use the
 logicanalyzer to visualize the input/output behavior of your system. Anotherdebugging technique is to dump {the state index, the input, the output} eachtime through the FSM controller to create a log of the operation

During the simulationgrading I will automatically set the inputs and check your outputs.**Hint: I recommend reducing thewait times for all states to be less than a second, so the simulation gradingruns faster.**Once you get a 100 in simulation, you can increasethe wait times for the real board to be more reasonable.




## 实验代码

我只想说：高详细注解版你们开不开心！

好吧……其实并没有人来看……

Code：


```cpp
// ***** 0. Documentation Section *****  
// TableTrafficLight.c for Lab 10  
// Runs on LM4F120/TM4C123  
// Index implementation of a Moore finite state machine to operate a traffic light.    
// Daniel Valvano, Jonathan Valvano  
// November 7, 2013  
  
// east/west red light connected to PB5  
// east/west yellow light connected to PB4  
// east/west green light connected to PB3  
// north/south facing red light connected to PB2  
// north/south facing yellow light connected to PB1  
// north/south facing green light connected to PB0  
  
// pedestrian detector connected to PE2 (1=pedestrian present)  
// north/south car detector connected to PE1 (1=car present)  
// east/west car detector connected to PE0 (1=car present)  
  
// "walk" light connected to PF3 (built-in green LED)  
// "don't walk" light connected to PF1 (built-in red LED)  
  
// ***** 1. Pre-processor Directives Section *****  
#include "TExaS.h"  
#include "tm4c123gh6pm.h"  
  
// ***** 2. Global Declarations Section *****  
#define SENSOR  (*((volatile unsigned long *)0x4002400C))  
#define LIGHT   (*((volatile unsigned long *)0x400050FC))  
  
// Linked data structure  
struct State   
{  
	unsigned long Out;            //Lights_Set  
	unsigned long OutPF;          //Walk_Set  
	unsigned long Time;           //Time Duration  
	unsigned long Next[8];        //Next[X] X means PE2-0 (0x07-0x00)  
};  
  
typedef const struct State STyp;  
  
#define goW         0             
#define waitW       1                 
#define goS         2                 
#define waitS       3                 
#define walk        4                 
#define notWalk     5         
#define walkOff     6             
#define notWalk2  	7           
#define walkOff2  	8           
  
STyp FSM[9]=  
{     
//{OUT,OutPF,Time,{    None,	 PE0,     PE1,   PE1&0,     PE2,   PE2&0,   PE2&1, PE2&1&0}}
 {0x0C, 0x02, 100,{     goW,     goW,   waitW,   waitW,   waitW,   waitW,   waitW,   waitW}},   //goW  
 {0x14, 0x02, 60, {   waitW,     goW,     goS,     goS,    walk,    walk,     goS,     goS}},   //waitW  
 {0x21, 0x02, 100,{     goS,   waitS,     goS,   waitS,   waitS,   waitS,   waitS,   waitS}},   //goS  
 {0x22, 0x02, 60, {   waitS,     goW,     goS,     goW,    walk,    walk,    walk,    walk}},   //waitS  
 {0x24, 0x08, 100,{    walk, notWalk, notWalk, notWalk,    walk, notWalk, notWalk, notWalk}},   //walk  
 {0x24, 0x02, 60, { notWalk, walkOff, walkOff, walkOff, walkOff, walkOff, walkOff, walkOff}},   //notWalk  
 {0x24, 0x00, 60, { walkOff,notWalk2,notWalk2,notWalk2,notWalk2,notWalk2,notWalk2,notWalk2}},   //walkOff  
 {0x24, 0x02, 60, {notWalk2,walkOff2,walkOff2,walkOff2,walkOff2,walkOff2,walkOff2,walkOff2}},   //notWalk2  
 {0x24, 0x00, 60, {walkOff2,     goW,     goS,     goW,    walk,     goW,     goS,     goW}}    //walkOff2  
};  
  
unsigned long S;  // index to the current state  
unsigned long Input;  
  
// FUNCTION PROTOTYPES: Each subroutine defined  
void DisableInterrupts(void); // Disable interrupts  
void EnableInterrupts(void);  // Enable interrupts  
  
// ***** 3. Subroutines Section *****  
  
void SysTick_Init(void){  
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup  
	NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock  
}  
  
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)  
void SysTick_Wait(unsigned long delay)  
{  
	NVIC_ST_RELOAD_R = delay-1; 			// number of counts to wait  
	NVIC_ST_CURRENT_R = 0;       			// any value written to CURRENT clears  
	while((NVIC_ST_CTRL_R&0x00010000)==0){} // wait for count flag  
}  
  
// 800000*12.5ns equals 10ms  
void SysTick_Wait10ms(unsigned long delay)
{  
	unsigned long i;  
	for(i=0; i<delay; i++)
		SysTick_Wait(800000);  // wait 10ms  
}  
  
void PortBEF_Init(void){ volatile unsigned long delay;  
	SYSCTL_RCGC2_R |= 0x32;               	    // 1) B E F  
	delay = SYSCTL_RCGC2_R;               	    // 2) no need to unlock  
      
	GPIO_PORTE_AMSEL_R    &= ~0x07;           	// 3) disable analog function on PE2-0  
	GPIO_PORTE_PCTL_R     &= ~0x00000FFF; 		// 4) enable regular GPIO  
	GPIO_PORTE_DIR_R      &= ~0x07;       		// 5) inputs on PE2-0  
	GPIO_PORTE_AFSEL_R    &= ~0x07;         	// 6) regular function on PE2-0  
	GPIO_PORTE_DEN_R      |=  0x07;         	// 7) enable digital on PE2-0  
  
	GPIO_PORTB_AMSEL_R    &= ~0x3F;           	// 3) disable analog function on PB5-0  
	GPIO_PORTB_PCTL_R     &= ~0x00FFFFFF; 		// 4) enable regular GPIO  
	GPIO_PORTB_DIR_R      |=  0x3F;           	// 5) outputs on PB5-0  
	GPIO_PORTB_AFSEL_R    &= ~0x3F;           	// 6) regular function on PB5-0  
	GPIO_PORTB_DEN_R      |=  0x3F;           	// 7) enable digital on PB5-0  
  
	GPIO_PORTF_AMSEL_R    &= ~0x0A;       		// 3) disable analog on PF1, PF3  
	GPIO_PORTF_PCTL_R     &= ~0x0000F0F0; 		// 4) PCTL GPIO on PF1, PF3  
	GPIO_PORTF_DIR_R      |=  0x0A;       		// 5) PF3, PF1 out  
	GPIO_PORTF_AFSEL_R    &= ~0x0A;       		// 6) disable alt funct on PF1, PF3  
	GPIO_PORTF_DEN_R      |=  0x0A;       		// 7) enable digital I/O on PF1, PF3  
}  
  
int main(void)
{   
	TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210); // activate grader and set system clock to 80 MHz  
	SysTick_Init();     		
	PortBEF_Init();   		// initialize PB,PE,PF  
	S = goW;                // let goW be First State  
	EnableInterrupts();  
	while(1)  
	{  
		LIGHT = FSM[S].Out;                               	// set lights  
		GPIO_PORTF_DATA_R =                                	// set PFs into PortF's Data  
		(GPIO_PORTF_DATA_R & (~0x0A)) | FSM[S].OutPF;     
        
		SysTick_Wait10ms(FSM[S].Time);          			// Wait a Duration to show this state    
        Input = GPIO_PORTE_DATA_R;                  		// Get Sensor Info  
		S = FSM[S].Next[Input];                         	// Change the FSM  
	}
	return 0;
}
```









## 调试及机器检验过程



```
Running with Code Size Limit: 32K
Load "F:\\【资料】重大教学资料\\嵌入式系统及应用\\Labware\\Lab10_TrafficLight\\Lab10.axf" 

*** Restricted Version with 32768 Byte Code Size Limit
*** Currently used: 18876 Bytes (57%)

Start grading
Clock rate appears to be : 80 MHz
Running 5 tests

0) Initialization tests :

 - (PE2-0) have been selected as the input pins
 - Verifying input configuration...
   Pass: PORTE DEN bits (2-0) are high
   Pass: PORTE DIR bits (2-0) are low
   Pass: PORTE AFSEL bits (2-0) are low
   Pass: PORTE AMSEL bits (2-0) are low
   Pass: PORTE PCTL bits (11-0) are low

 - (PB5-0) have been selected as the output pins
 - Verifying output configuration...
   Pass: PORTB DEN bits (5-0) are high
   Pass: PORTB DIR bits (5-0) are high
   Pass: PORTB AFSEL bits (5-0) are low
   Pass: PORTB AMSEL bits (5-0) are low
   Pass: PORTB PCTL bits (23-0) are low

1) Servicing all 3 requests, lights should make a complete cycle
   FSM: Transition: 2 to 3
   FSM: Transition: 3 to 4
   FSM: Transition: 4 to 5
   FSM: Transition: 5 to 1
   FSM: Transition: 1 to 6
   FSM: Transition: 6 to 1
   FSM: Transition: 1 to 7
   FSM: Transition: 7 to 1
   FSM: Transition: 1 to 7
   FSM: Transition: 7 to 9
   FSM: Transition: 9 to 2
   Pass: All requests were serviced
   Pass: South request was serviced
   Pass: West request was serviced
   Pass: Walk request was serviced
 - Test PASSED

2) Servicing walk button, walk light should come on
   FSM: Transition: 2 to 3
   FSM: Transition: 3 to 1
   FSM: Transition: 1 to 6
   Pass: All requests were serviced
   Pass: Walk request was serviced
 - Test PASSED

3) Servicing west button, west green light should come on
   FSM: Transition: 6 to 1
   FSM: Transition: 1 to 7
   FSM: Transition: 7 to 1
   FSM: Transition: 1 to 7
   FSM: Transition: 7 to 9
   FSM: Transition: 9 to 2
   Pass: All requests were serviced
   Pass: West request was serviced
 - Test PASSED

4) Servicing south button, south green light should come on
   FSM: Transition: 2 to 3
   FSM: Transition: 3 to 4
   Pass: All requests were serviced
   Pass: South request was serviced
 - Test PASSED

Done grading. Score is 100
```



![](https://img-blog.csdn.net/20150504191656717?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)












