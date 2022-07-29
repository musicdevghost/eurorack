# Leo Leo - The Matrix Programmer

<img src="./Images/frontpanel_preview.png" height="400px">

Features:

"Leo Leo" (faster in thai) is a programmer/sequencer module based on the first original Serge Programmer from the 70s, after discussing with Elby Designs and Serge himself who gave me open direction for this project, alongside some recommendations, I am happy to share here my evolution of the Programmer as an open source DIY project, this is quite the module and requires proper context, demonstration and education, below are a list of videos I've made myself to cover all the different aspects of this module.

To make it simpler for now, think of it as a, 4x4, 2x8, 2x16 analog Step Sequencer with individual CV outs per stage/step plus special sequential outs as well as a "ABCD" and "DCBA" outputs.

There's no notion of clock, so there's no internal clocking, you have inputs to make the programmer active from left to right, right to left and/or top to bottom and on each direction you take you can define rules per stage/step on what should happen (skip, mute, stop, play).

This is module to explore for sure, if I have you interest still, then please feel free to browse around my list of videos below about this special module.

Demos: TBD

## Build Instructions
* You can easiliy order all needed components from [Tayda](https://www.taydaelectronics.com) online shop simply by uploading the ["Tayda Auto Order"](./Tayda%20Auto%20Order.csv) CSV file at https://www.taydaelectronics.com/quick-order/
* Print out the BOM file
* Please keep in mind that also the frontpanel should be produced in FR4 material when ordering it and not in aluminium (Just use the same material to build the PCBs and Front Panel) since the lightning effect only works with the FR4 material
* Follow the build instructions video at: TBD
* Follow the calibration instructions video at: TBD

## Developing
If you want to improve on the design and contribute you'll first need to import the project files into your workspace, I used EasyEDA for all my projects, but if you're using something else I believe by now these projects can be imported from a couple of platforms, I use EasyEDA because it's simpler to me, I can just order the PCB directly from the application and it just works (I am by no means sponsored or supported from the company in any way, just sharing what works for me).

You can fork this repository, work on your own improvements, adjustments and hopefuly you would submit a PR for your changes to be merged into this main repository, thus sharing with everyone your updates as well, after of course sharing review of the changes and testing.

You can use [EasyEDA](https://easyeda.com/) to open all project files and import into your own workspace by importing the files located inside the "EasyEDA" folder, from the application you can make your adjustments, and update the schematics PDF files and Gerber files where available and update the repository with your changes.

## Panel Skeleton Overview
![panel](./Images/panel_preview.png)

## Specs

TBD

## Disclaimer
All electronic projects and designs shared on this repository, or associated web sites should considered dangerous if not lethal if not used safely. When working on projects based on these designs, use extreme care to ensure that you do not come into contact with mains AC voltages or high voltage DC. If you are not confident about working with mains voltages, or high voltages, or you are not legally allowed to work with mains voltages, or high voltages, you are advised not to attempt work on them. The author, host, and all contributors associated with these repositories disclaim any liability for damages should anyone be killed or injured while working on these projects, or projects based on these designs, or any other project or design presented on these repositories and any associated repositories. The author, host, and all contributors associated with these repositories also disclaim any liability for projects, or projects based on these designs, or any other project or design presented on these repositories and any associated repositories when used in such a way as to infringe relevant government regulations and by-laws. 