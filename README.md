# Typing the Technical Interview (in C++)

Re-implementation of the https://aphyr.com/posts/342-typing-the-technical-interview in C++.

## How to run

Build to run compile-time unit tests:

```bash
$ clang++ --std=c++17 -c ttti.cc
```

Solve for N <= 8 with:

```bash
$ clang++ --std=c++17 -c ttti.cc -DNQUEENS=N6
```

This will fail on assertion error and print the type of `Solution<N>`
which encodes the board as a list of queen positions. (For N={2,3} this will succeed without any output, as there are no valid solutions.)

For "graphical" rendering try this shitty bash thing:
```bash
$ clang++ --std=c++17 -c ttti.cc -DNQUEENS=N6 2>&1 | \
    grep -E 'static.*assert(ion)? failed' | \
    sed -E 's/.*IsSameType<(.+), Nil>.*/\1/' | \
    sed 's/, Cons/\n/g' | \
    sed -E 's/.*, ([S<]*)Z.*/\1#/g' | \
    tr -d 'S' | tr '<' ' '
```

## Performance

Surprisingly not too bad. On my machine N=7 takes around a second and N=8 takes about five seconds.

## License

It is not clear what the license of Kyle Kingsbury's original post is, and this code probably is derivative work, so I'm not claiming any rights over it and don't provide a license.