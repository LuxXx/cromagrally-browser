// Runs before the game's main(). Mount persistent storage for prefs & saves.
Module.preRun = Module.preRun || [];
Module.preRun.push(function () {
  var dir = '/home/web_user/.config';
  FS.mkdirTree(dir);
  FS.mount(IDBFS, { autoPersist: true }, dir);
  addRunDependency('idbfs');
  FS.syncfs(true, function (err) {
    if (err) console.warn('Could not load saved data:', err);
    removeRunDependency('idbfs');
  });
});
