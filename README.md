# File md5sum tool

## Usage
```
$ md5sum
File md5sum tools
Usage: md5sum [options] <file>...
Options:
    -h,--help                        Show this help message.
    -o <OFFSET>,--offset=<OFFSET>    Offset of file to calc md5.
    -l <LENGTH>,--length=<LENGTH>    Length of file from offset.
```

## Example
```bash
$ md5sum -o 100 -l 1010 testfile]
(main:118) Get file [testfile]] size [1048576], offset [100], length [1010]!
(main:136) md5sum of file [testfile]:
5d2bea0395ada17075c41e670c649d4c

$ md5sum testfile 2>/dev/null
1bafd1cd9ed31a9d8864623812881a9e
```
