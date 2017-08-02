# Markdown Parser 

## Compile
```
just make or cmake, it offer both Makefile and CMakeLists
```

## Use
markdown  [options] source_file

|options|function|
|-|-|
|-h| show you the help informations|
|-v| show you the version informations|
|-p| convert to pdf file|
|-o| set the output file name，default name is generated using soure_file| 

## Grammer Support  
- title 
```
# ---------h1
## ---------h2
### --------h3
#### -------h4
##### ------h5
###### -----h6
```
- reference
```
'>' is at begin of line 
```
- line 
```
At least three '-' or '*' are needed in the top of line:
***
---
```
- bold
```
if you want to make text bold just write like : **test** 
test will change into bold
```
- italic
```
use like bold but just one '*': *test*, if you use three '*' it will be bold and italic
```
- image link
```
add a image link：![iamge](url)
```
- hyper link
```
a normal link: [text](url)
```
- unordered list
```
with the start symbol '+' or '-' or '*' and a blank ' ',just like :
- test1
- test2
```
- ordered list
```
begin with 'number. ' 
```

- code block
```
code block begin with "```" and end with "```" 
now it can not support language recogniton
```

- table 
```
if you want to use a table ,first line should include '|', the second line to set the format just like: "|-|-|",each table element should be divided by '|'. a example:
|th1|th2|
|-|-|
|first|1|
|second|2|
and below is also ok:
th1|th2
-|-
first|1
second|2
it will show like below:
```
th1|th2
-|-
first|1
second|2

- to pdf
```
convert to pdf file is supported by wkhtmltopdf, you need to download it an install
```

## TO DO
```
support math formula and more grammer
```
