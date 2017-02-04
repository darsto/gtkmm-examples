# Multi windowed GTKmm3 example
Based on the gtkmm-multithread-example. 

This example features internal window manager. It provides simple interface to create windows and manages the application lifetime by keeping track of shown & hidden windows.

Note that the "windows" in this context are completely independent from each other. They all could be considered by the user as separate applications. This example does not contain any dependent windows (i.e. dialogs).