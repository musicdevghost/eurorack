# Eurorack DIY

## Open Source Eurorack Modules

### Quick Build Guide

*Every project shares Gerber files (PCB files), EasyEDA project files (Design files) apart from other kinds of information and instructions, so that you can truly make each module your own, you can change the front panel for example and add your own twist to it, or change the PCB itself and the circuit if you wish so.*

1. Download the Gerber zip files from the module in question, example: [Download Kra Pao Gerber Files](https://github.com/musicdevghost/eurorack/tree/main/KRA%20PAO%20-%20RESONANT%20LOW%20PASS%20GATE/Gerber%20Files)
2. Browse to your online local PCB manufacturer and upload each zip file one by one, I happen to use https://cart.jlcpcb.com/
    - Don't forget to check the "Remove Order Number" option just for the front panels
3. At the same time make your component order, each project has a **BOM.xls** file where you can see details for every component including a reference link to what component exactly I'm using for my builds, example: [Kra Pao BOM File](https://github.com/musicdevghost/eurorack/blob/main/KRA%20PAO%20-%20RESONANT%20LOW%20PASS%20GATE/BOM.xlsx)
4. If you happen to use [Tayda](https://www.taydaelectronics.com/) I have also an auto cart (CSV file) that you can just upload and checkout for all needed components, example: [Download Kra Pao Tayda Cart CSV File](https://github.com/musicdevghost/eurorack/blob/main/KRA%20PAO%20-%20RESONANT%20LOW%20PASS%20GATE/Tayda%20Auto%20Order.csv) then just upload it at https://www.taydaelectronics.com/quick-order/
    - Some modules wil require components not easily available like Vactrols for the Kra Pao module, tipically anything that I cannot find on Tayda I will order from [Thonk UK](https://www.thonk.co.uk/), but I would say that 90% of all module components are easily reachable on Tayda/Mouser online stores
5. Time to build! On every project there's always video instructions step by step on how to build your new module, enjoy!

### Production ready
All these modules have been built and fully tested in a live environment, documentation and build instructions are available for every module, alongside a detailed BOM file with links to all mentioned components and an auto cart file ready for [Tayda online shop](https://www.taydaelectronics.com/).
| Module  | Description |
| ------------- | ------------- |
| [Su Su - Bipolar Power Supply](https://github.com/musicdevghost/eurorack/tree/main/SU%20SU%20-%20POWER%20SUPPLY)  | Provides 1A, +/-12v  |
| [Tom Yum - VCO](https://github.com/musicdevghost/eurorack/tree/main/TOM%20YUM%20-%20VCO)  | Fully analog voltage controlled oscillator with stable v/oct tracking  |
| [Kra Pao - Resonant Low Pass Gate](https://github.com/musicdevghost/eurorack/tree/main/KRA%20PAO%20-%20RESONANT%20LOW%20PASS%20GATE)  | Buttery resonant LPG/VCA/VCF based on the original design from Thomas White, core design used with permission  |
| [Cha Cha - VCLFO](https://github.com/musicdevghost/eurorack/tree/main/Cha%20Cha%20-%20VCLFO) | 6HP Voltage Controlled LFO with 4 wave outputs and reset input  |
| [Sigma](https://github.com/musicdevghost/eurorack/tree/sigma/Sigma%20-%20Function%20Generator) | Function/Slope Generator with V/OCT tracking  |

### Work in progress
You might notice that there are and will be a couple of branches from the main one, these represent ongoing work and development and considered not ready for production mode just yet, so if you intend to build one of the mentioned designs keep in mind it might have bugs or not work at all, the ones in the main branch have been all tested and are ready for production.
| Module  | Description |
| ------------- | ------------- |
| [Illusions](https://github.com/musicdevghost/eurorack/tree/fx-dsp-space-time/Illusions%20-%20Space%20%26%20Time%20FX)  | DSP Space & Time FX Unit  |
| [Mak Mak](https://github.com/musicdevghost/eurorack/tree/mak-mak/Mak%20Mak%20-%20QUAD%20VCA-ATT-MIXER)  | Quad VCA/Attenuator or 3 Channel mixer  |
| [Leo Leo - The Matrix Programmer](https://github.com/musicdevghost/eurorack/tree/leo-leo/LEO%20LEO%20-%20MATRIX%20STAGE%20PROGRAMMER) | Serge inspired Programmer on steroids  |

## Contributing
If you want to improve on the design and contribute you'll first need to import the project files into your workspace, I used EasyEDA for all my projects, but if you're using something else I believe by now these projects can be imported from a couple of platforms, I use EasyEDA because it's simpler to me, I can just order the PCB directly from the application and it just works (I am by no means sponsored or supported from the company in any way, just sharing what works for me).

You can fork this repository, work on your own improvements, adjustments and hopefuly you would submit a PR for your changes to be merged into this main repository, thus sharing with everyone your updates as well, after of course sharing review of the changes and testing.

You can use [EasyEDA](https://easyeda.com/) to open all project files and import into your own workspace by importing the files located inside the "EasyEDA", from the application you can make your adjustments, and update the schematics PDF files and Gerber files where available and update the repository with your changes.

## Disclaimer
All electronic projects and designs shared on this repository, or associated web sites should considered dangerous if not lethal if not used safely. When working on projects based on these designs, use extreme care to ensure that you do not come into contact with mains AC voltages or high voltage DC. If you are not confident about working with mains voltages, or high voltages, or you are not legally allowed to work with mains voltages, or high voltages, you are advised not to attempt work on them. The author, host, and all contributors associated with these repositories disclaim any liability for damages should anyone be killed or injured while working on these projects, or projects based on these designs, or any other project or design presented on these repositories and any associated repositories. The author, host, and all contributors associated with these repositories also disclaim any liability for projects, or projects based on these designs, or any other project or design presented on these repositories and any associated repositories when used in such a way as to infringe relevant government regulations and by-laws. 
