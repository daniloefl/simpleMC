# simpleMC

This program uses the Monte Carlo technique to simulate electron-positron scattering
according to the probability distribution of either the Bhabha scattering or full electroweak
electron-positron scattering.

The code documentation can be found in <https://daniloefl.github.io/simpleMC/html/>.
You can see a graphical result of the simulations here <https://daniloefl.github.io/simpleMC/SimpleMC3d.html>.

The Analysis program can be used to convert the text file generated into a ROOT file
for analysis into ROOT. However, the original MC generator is ROOT independent.
Therefore, if one does not have access to ROOT, this can be used anyway with a different
software being used to generate the plots.

```
make
```

One can execute the program, for example, by doing:

```
./generate -f simulation.txt -e 1000000 -E 45 -u
./Analysis -f simulation.txt -o output.root
```

where the generate command produces the simulation.txt file with one electron-positron
collision simulation per line. The line contents are described in the first line, which is
to be ignored. The Analysis command, reads the generated file and produces a ROOT file output,
which can be used to make plots easily. In generate, the -e flag indicates the number of
collisions to simulate, the -E flag indicates the energy of each colliding beam and the -u flag
indicates that the events should be unweighted, so that each simulation has the same weight and
resembles a real collision.

One particularly interesting option is -s [VALUE], which varies the centre-of-mass energy
randomly by +/- [VALUE]. It allows for a scan of the Z mass peak.

One can see more information on both commands in their help flag:

```
$ ./generate -h
This program simulates scattering of electron-positron -> Z -> e+e-.
All four-momenta are written, one per line of the output text file.

Usage:
generate [options]
where [options] are one of:

   -h  | --help                Show this help message.
   -f  | --file [FILE]         Write results in file [FILE].
   -e  | --events [N]          Produce [N] events before unweighting.
   -E  | --energy [VALUE]      Simulate electron-positron collisions with center-of-mass energy given by [VALUE] GeV.
   -u  | --unweight            If this flag is sent, apply unweighting.
   -p  | --process [NAME]      'Bhabha' for the electron-positron -> photon -> electron-positron QED scattering and 'EWK' to also include Z boson mediators.
   -s  | --scanCoM [SIGMA]     Vary centre-of-mass according to a uniform distribution of width [SIGMA].
```

```
$ ./Analysis -h
This program reads a text file with simulations of electron-positron scattering produced by generate and produces a ROOT file with that information for easy plotting.

Usage:
Analysis [options]
where [options] are one of:

   -h  | --help             Show this help message.
   -f  | --file [FILE]      Input in file [FILE].
   -o  | --output [FILE]    Output ROOT file to be written in [FILE].
   -e  | --events [N]       Read up to [N] events.

```

The implementation of the cross section calculation can be seen in the ProcessComp.cxx
and BhabhaProcess.cxx files.

-------------------------
Danilo Ferreira de Lima <daniloefl@gmail.com>
