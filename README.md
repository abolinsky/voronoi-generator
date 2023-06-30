# voronoi-generator

[candy manhattan](img/candy_manhattan.png)

## build
```
g++ -std=c++20 -Wall -Werror -Wextra -fno-exceptions *.cpp -o voronoi
```

## run
```
./voronoi 3840 2160 500 -p palettes/candy > image.ppm
open image.ppm
```

## help
```
./voronoi -h
```
