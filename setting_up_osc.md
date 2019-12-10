# Step 1. Set up your raspberry pi as a Wireless Access Point

To set up your raspberry pi zero W as a wireless access point, [follow these instructions](https://www.raspberrypi.org/documentation/configuration/wireless/access-point.md).

# Step 2. Install Node on the raspberry pi

So first of all type this in your terminal on raspberry pi to detect the version that you need:

```uname -m```

If the response starts with armv6 than that's the version that you will need. For example for raspberry pi zero W you will need ARMv6

The latest version that supports armv6 is v.10.x, go to [Node's official website](https://nodejs.org/en/download/releases/) and download the tar.gz file that you need. 

In my case it was: [node-v10.17.0-linux-armv6l.tar.gz](https://nodejs.org/dist/latest-v10.x/node-v10.17.0-linux-armv6l.tar.gz) 

You can download the file either by doing (if your pi is connected to the internet): 

```wget https://nodejs.org/dist/latest-v10.x/node-v10.17.0-linux-armv6l.tar.gz```

or 

```curl -o node-v10.17.0-linux-armv6l.tar.gz https://nodejs.org/dist/latest-v10.x/node-v10.17.0-linux-armv6l.tar.gz```

or doing ssh into the pi and copying the files to the pi from the terminal of your computer:

```"C:\Program Files\PuTTY\pscp.exe" node-v10.17.0-linux-armv6l.tar.gz pi@192.168.4.1:/home/pi```

Then extract the file:

```tar -xzf node-v10.17.0-linux-armv6l.tar.gz```

Finally copy node to its final location and expose it as a global command:
```
cd node-v6.11.1-linux-armv6l/
sudo cp -R * /usr/local/
```
