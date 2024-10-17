This is the readme for the model associated with the paper:

Goldman MS, Golowasch J, Marder E, Abbott LF (2001) Global structure,
robustness, and modulation of neuronal models. J Neurosci 21:5229-38

This code was supplied by Mark Goldman.

The raw code is in Microsoft Visual C++.  I also have included a
compiled .exe file for those don't use Visual C++; you can change
conductance values and compartment geometry with the interface, but
you won't be able to change channel kinetics because these are
hardwired in the code.  It takes some interpretation, as I was
building my own mini-Neuron version when i wrote it and there are many
parts of the interface that actually are placeholders, i.e. you can
click all you like on the Sensors or the Synapses, but they don't
actually do anything!  (It was many years ago, but I think it does run
2 compartments successfully). It uses an adaptive step size algorithm
that was tailored for the stg simulations: This is what the minimum
and maximum step sizes were used for, and they dramatically speed up
the simulations because there are rapid dynamics during bursts and
spikes, versus very slow changes in variables during interburst
intervals.  All bets are off if someone uses this for non-stg
simulations.

Some other idiosyncratic things are that it actually remembers the
location of your windows on your screen (see documentation file for
how to fix if they ever get screwed up).  The main window is not used
except for the File Menu. The "3 blind mice..." (see how they run...)
window may need to be resized to show the graphs.  The display tab can
be used to add more graphs and change axes limits, and available
variables can be added in code.

Additional notes:
1) for the final data analysis, the identification of "silent, tonic,
burst" is the "extracellular" characterization. As explained in the
2001 paper, however, I used the transmission estimate (also reported)
to decide whether single-spikers were "tonic" vs. "1-spike bursts"
(basically whether the spike rides on top of a slow calcium wave).

2) when loading in files or using the drop boxes to update things, one
should always FIRST make the change(s), and then click "Apply" to make
them active. This allowed me, online, to change sets of parameters as
I explored the model and hopefully will be insightful for others.

3) Use the Windows file menus to read in the SavedCases corresponding
to some of the figures in Goldman et al 2001 and Golowasch et al 2002.

4) It seems to produce an error after a run finishes on my new Windows
7 computer, but it does run all the way through successfully (i.e. you
just can't run multiple files, like I could on my old operating
system).

5) For (a little) more documentation, see Documentation.txt in the
ZhengModel2 folder.
