# GTKmm3 examples

Each branch contains a single example.

* [Multi-threaded example](https://github.com/darsto/gtkmm-examples/tree/gtkmm-multithread-example)
* [Multi-windowed example](https://github.com/darsto/gtkmm-examples/tree/gtkmm-multiwindow-example)

Also worth noticing, here are some util classes used by these examples:
* [[util] SimpleColumnModel](https://gist.github.com/darsto/098b104994542f09fd91e32ebf7c15b3) - A tuple-like wrapper for Gtk::TreeModel::ColumnRecord
* [[util] ConcurrentQueue](https://gist.github.com/darsto/9876f7337d374b630c2dee0b07bef043) - Semi-lockless single-producer single-consumer queue with always-successful push(T&)