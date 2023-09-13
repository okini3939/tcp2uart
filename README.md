# TCP to UART bridge for NanoPi

- NanoPi NEO/NEO3/etc.
- FriendlyCore Xenial 4.14 (based on UbuntuCore)
- https://wiki.friendlyelec.com/wiki/index.php/NanoPi_NEO

/dev/ttyS <===> TCP/IP

```
% make

% ./tcp2uart -d<DEVICE> -p<PORT>

% ./tcp2uart -d/dev/ttyS2 -p10000
```
