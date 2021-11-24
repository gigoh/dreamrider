AFRAME.registerComponent("audioanalyser-volume-scale", {
  schema: {
    analyserEl: { type: "selector" },
    multiplier: { type: "number", default: 1 },
  },

  tick: function () {
    var analyserEl = this.data.analyserEl || this.el;
    var analyserComponent;
    var el = this.el;
    var el2 = document.querySelector(".environmentDressing");
    var volume;

    analyserComponent = analyserEl.components.audioanalyser;
    if (!analyserComponent.analyser) {
      return;
    }

    volume = analyserComponent.volume * this.data.multiplier;
    // el.setAttribute("scale", {
    //   x: 1 + volume,
    // });
    el2.setAttribute("scale", {
      y: 1 + volume,
    });
  },
});
