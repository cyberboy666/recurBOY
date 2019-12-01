
~~download the latest recur image - this has openframeworks and many other dependacies on it already .. (saves some time)~~

since we dont need the gui interface for this project and want it to be as light as possible i will do a fresh install from a stretch lite img.

## this part the same as the r_e_c_u_r setup

- downloaded the latest (~~2017-11-29~~ ~~2018-04-18~~ [2019-07-10]) raspbian-raspbian-lite image from offical site.

- flashed it to my sd using etcher

- set up auto console login and changed keyboard layout using `sudo raspi-config`

- following the beginning of [this](https://gist.github.com/kmpm/8e535a12a45a32f6d36cf26c7c6cef51) guide,
set up wifi and run all updates: 

`sudo nano /etc/wpa_supplicant/wpa_supplicant.conf` Add the bottom of the file
```
network={
  ssid="YOUR_SSID"
  psk="YOUR_PASSWORD"
}
```

then `sudo apt update` and `sudo apt upgrade` , sudo reboot

## install a few things

`sudo apt-get install -y git omxplayer python3-pip`

## update config.txt , cmdline.txt

`git clone https://github.com/langolierz/r_e_c_u_r`

the fastest way is to copy the config.txt i included in the r_e_c_u_r repo:

`sudo cp ~/r_e_c_u_r/dotfiles/config.txt /boot/config.txt`

add to end of cmdline.txt `sudo nano /boot/cmdline.txt` : 
`quiet splash logo.nologo plymouth.ignore-serial-consoles` for quiet boot with splash screen 

## install openframeworks and addons

`wget "https://openframeworks.cc/versions/v0.10.1/of_v0.10.1_linuxarmv6l_release.tar.gz"`

- `mkdir openFrameworks` and `tar vxfz of_v0.10.1_linuxarmv6l_release.tar.gz -C openFrameworks --strip-components 1`
- `cd openFrameworks/scripts/linux/debian/` &  `sudo ./install_dependencies.sh`
- `make Release -C ~/openFrameworks/libs/openFrameworksCompiled/project`
- `cd ~/openFrameworks/apps/myApps/` and `git clone https://github.com/langolierz/recurBOY`
- `cd ~/openFrameworks/addons/` and `git clone https://github.com/langolierz/ofxOMXCamera.git` (will swap this out for main once/if my edits work and get in)
- `git clone https://github.com/langolierz/ofxVideoArtTools`
- `git clone https://github.com/timscaffidi/ofxVideoRecorder` and its depend : `sudo apt-get install ffmeg` 
- `git clone https://github.com/jvcleave/ofxOMXPlayer` and install depends : `cd ofxOMXPlayer; ./install_depends.sh` and checkout last stable version `git checkout c826828`
- `git clone https://github.com/kashimAstro/ofxGPIO`
- `git clone https://github.com/danomatika/ofxMidi`
- `git clone https://github.com/jeffcrouse/ofxJSON`

- `make -C ~/openFrameworks/apps/myApps/recurBOY`

## recurBOY specific installs:

- from `cd ~/` , `mkdir Videos`, `mkdir Shaders`, `mkdir Fx`

then following the instructions on [this blog]

`sudo apt install build-essential python3-dev python3-smbus python3-pil python3-numpy`

then
```
sudo python3 -m pip install RPi.GPIO
sudo python3 -m pip install Adafruit_GPIO
```
and `sudo pip3 install python-osc`

from ~/
```
git clone https://github.com/cskau/Python_ST7735
cd Python_ST7735
sudo python3 setup.py install
```

add to the end of config.txt `sudo nano /boot/config.txt`

`dtoverlay=spi1-3cs`

and install the font :

`sudo apt-get install fonts-liberation`

## setup for ethernet over usb

add to end of cmdline.txt `sudo nano /boot/cmdline.txt` :

`modules-load=dwc2,g_ether`

add to end of config.txt `sudo nano /boot/config.txt` :

`dtoverlay=dwc2`

## usb mount

`sudo apt-get install usbmount`

open `sudo nano /lib/systemd/system/systemd-udevd.service` and change MountFlags=slave to MountFlags=shared

## cron job on startup

`crontab -e` and pick 2, at bottom of the file add the start command:

`@reboot sh /home/pi/openframeworks10.1/apps/myApps/recurBOY/startRecurBoy`

## still to do:

- splash screen


[this blog]: https://jakew.me/2018/01/19/st7735-pi/

## SSH onto the PI from Windows (Tested on Windows 8.1)

To log into the internal storage of the Raspberry Pi Zero, follow the instructions on [this website](http://www.circuitbasics.com/raspberry-pi-zero-ethernet-gadget/).

Once you are inside to copy files from your Windows computer to the internal Storage do: 

### For Videos:
```
"C:\Program Files\PuTTY\pscp.exe" your_video.mp4 your_video2.mp4 your_video3.mp4 pi@raspberrypi.local:/home/pi/Videos
```
You should replace `your_videoX.mp4` with the name of the video file name (including the extension) you want to copy to the internal storage. If you want to transfer more than one file put all the names separated by a space like in the example above.

### For Shaders:
```
"C:\Program Files\PuTTY\pscp.exe" your_shader.frag pi@raspberrypi.local:/home/pi/Shaders
```
You should replace `your_shader.frag` with the name of the shader file name (including the extension) you want to copy to the internal storage. If you want to transfer more than one file put all the names separated by a space like in the example above.
