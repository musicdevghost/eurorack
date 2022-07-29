# Sigma - Function Generator

<div>
    <p style="float:left;">
    Sigma is an analog function/slope generator based on Serge's first designs on the dual universal slope generator, here boiled down to one module in 8HP with a couple more outputs and an upgrade to improve v/oct tracking.
    </p>
    <p style="float:left;">
        <img src="./Images/panel_overview.png" height="400px">
    </p>
    <p style="clear:both;"></p>
</div>


Features:
* Voltage control over either Rise, Fall or Both through a switch control
* Separate V/OCT input/tracking
* Two processing inputs, Trigger and IN (IN takes any kind of signal input)
* Bipolar, DC and AC outputs
* EOC (End of cycle) outputs and logic NOT EOC output

Demos: TBD

## Build Instructions
* You can easiliy order all needed compoenents from [Tayda](https://www.taydaelectronics.com) online shop simply by uploading the ["Tayda Auto Order"](./Tayda%20Auto%20Order.csv) CSV file at https://www.taydaelectronics.com/quick-order/
* Print out the BOM file
* Follow the build instructions video at: TBD
* Follow the calibration instructions video at: TBD

## Developing
If you want to improve on the design and contribute you'll first need to import the project files into your workspace, I used EasyEDA for all my projects, but if you're using something else I believe by now these projects can be imported from a couple of platforms, I use EasyEDA because it's simpler to me, I can just order the PCB directly from the application and it just works (I am by no means sponsored or supported from the company in any way, just sharing what works for me).

You can fork this repository, work on your own improvements, adjustments and hopefuly you would submit a PR for your changes to be merged into this main repository, thus sharing with everyone your updates as well, after of course sharing review of the changes and testing.

You can use [EasyEDA](https://easyeda.com/) to open all project files and import into your own workspace by importing the files located inside the "EasyEDA" folder, from the application you can make your adjustments, and update the schematics PDF files and Gerber files where available and update the repository with your changes.

## Panel Skeleton Overview
![panel](./Images/panel_overview.png)

## Disclaimer
All electronic projects and designs shared on this repository, or associated web sites should considered dangerous if not lethal if not used safely. When working on projects based on these designs, use extreme care to ensure that you do not come into contact with mains AC voltages or high voltage DC. If you are not confident about working with mains voltages, or high voltages, or you are not legally allowed to work with mains voltages, or high voltages, you are advised not to attempt work on them. The author, host, and all contributors associated with these repositories disclaim any liability for damages should anyone be killed or injured while working on these projects, or projects based on these designs, or any other project or design presented on these repositories and any associated repositories. The author, host, and all contributors associated with these repositories also disclaim any liability for projects, or projects based on these designs, or any other project or design presented on these repositories and any associated repositories when used in such a way as to infringe relevant government regulations and by-laws. 