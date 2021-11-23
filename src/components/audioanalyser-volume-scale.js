AFRAME.registerComponent("audioanalyser-volume-scale", {
  schema: {
    analyserEl: { type: "selector" },
    multiplier: { type: "number", default: 1 },
  },

  tick: function () {
    var analyserEl = this.data.analyserEl || this.el;
    var analyserComponent;
    var el = this.el;
    var volume;

    el = document.querySelector(".environmentDressing");

    analyserComponent = analyserEl.components.audioanalyser;
    if (!analyserComponent.analyser) {
      return;
    }

    volume = analyserComponent.volume * this.data.multiplier;
    el.setAttribute("scale", {
      // x: volume,
      y: 1 + volume,
      // z: volume
    });
  },
});
