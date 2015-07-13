# libaap

Tools for working with the Apple Accessory Protocol

## Note

I have no intention of developing this code any further. I originally wrote it
so I could make sense of the data going back and forth on between my phone and
my car.

## Usage

* Run `make` to build `input` and `inspect` binaries
* Run `./input > /dev/ttyUSB0` to manually write custom frames to `/dev/ttyUSB0`
* Run `./inspect < /dev/ttyUSB0 > /dev/null` to read frames from `/dev/ttyUSB0` and display debugging output

Combine these tools to verify how to build frames:

```
$ ./input | ./inspect > /dev/null
0x04 0x00 0x26 0x01
17:25:08.330699 mode 4 message (0026): Start streaming mode
```

## License

Simplified BSD
