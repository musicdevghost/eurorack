# Kra Pao - Resonant Low Pass Gate

<img src="./Images/3d_faceplate.png" height="400px">

Features:

A resonant low pass gate at is core, though it can function in three different modes, as a VCA, a LPG or a VCF (Low Pass).

* It has two modulation inputs, one unipolar and other bipolar.
* An extra dampening control
* A toggle switch to switch operation from LPG, VCA and VCF
* On its own the module can resonate, it won't follow v/oct standard but it can be played.

Used with permission from Thomas White, the author of the core design that this module is based.

Demos: https://www.youtube.com/watch?v=gdqPUZgIWGM&ab_channel=Voxmachina

## Build Instructions

For this module you will need to use Vactrols, these are not easily accessible these days, although at the time of this writing I've managed to get mine from [Thonk](https://www.thonk.co.uk/), or you can also build your own vactrols as seen on this video for example: https://youtu.be/8BOnvSQpoGY

* Download the Gerber zip files inside the [Gerber Files](https://github.com/musicdevghost/eurorack/tree/main/KRA%20PAO%20-%20RESONANT%20LOW%20PASS%20GATE/Gerber%20Files) folder
* Browse to your online local PCB manufacturer and upload each zip file one by one, I happen to use https://cart.jlcpcb.com/
    - Don't forget to check the "Remove Order Number" option just for the front panels
* You can easiliy order all the other needed components from [Tayda](https://www.taydaelectronics.com) online shop simply by uploading the ["Tayda Auto Order"](./Tayda%20Auto%20Order.csv) CSV file at https://www.taydaelectronics.com/quick-order/
* Print out the BOM file
* Follow the build instructions video at: https://youtu.be/-vEFokdSwIU

## Developing
If you want to improve on the design and contribute you'll first need to import the project files into your workspace, I used EasyEDA for all my projects, but if you're using something else I believe by now these projects can be imported from a couple of platforms, I use EasyEDA because it's simpler to me, I can just order the PCB directly from the application and it just works (I am by no means sponsored or supported from the company in any way, just sharing what works for me).

You can fork this repository, work on your own improvements, adjustments and hopefuly you would submit a PR for your changes to be merged into this main repository, thus sharing with everyone your updates as well, after of course sharing review of the changes and testing.

You can use [EasyEDA](https://easyeda.com/) to open all project files and import into your own workspace by importing the files located inside the "EasyEDA" folder, from the application you can make your adjustments, and update the schematics PDF files and Gerber files where available and update the repository with your changes.

## Panel Skeleton Overview
![panel](./Images/3d_faceplate.png)

## Specs

* 12HP
* 15mA (+/-12v)

## Disclaimer
All electronic projects and designs shared on this repository, or associated web sites should considered dangerous if not lethal if not used safely. When working on projects based on these designs, use extreme care to ensure that you do not come into contact with mains AC voltages or high voltage DC. If you are not confident about working with mains voltages, or high voltages, or you are not legally allowed to work with mains voltages, or high voltages, you are advised not to attempt work on them. The author, host, and all contributors associated with these repositories disclaim any liability for damages should anyone be killed or injured while working on these projects, or projects based on these designs, or any other project or design presented on these repositories and any associated repositories. The author, host, and all contributors associated with these repositories also disclaim any liability for projects, or projects based on these designs, or any other project or design presented on these repositories and any associated repositories when used in such a way as to infringe relevant government regulations and by-laws. 
