AFRAME.registerComponent("audioanalyser-volume-scale", {
  schema: {
    analyserEl: { type: "selector" },
    multiplier: { type: "number", default: 1 },
  },

  tick: function () {
    var analyserEl = this.data.analyserEl || this.el;
    var analyserComponent;
    // var el = this.el;
    var el = document.querySelector(".environmentDressing");
    var el2 = document.querySelector(".environmentDressing2");
    var el3 = document.querySelector(".environmentDressing3");
    var volume;

    analyserComponent = analyserEl.components.audioanalyser;
    if (!analyserComponent.analyser) {
      return;
    }

    volume = analyserComponent.volume * this.data.multiplier;
    el.setAttribute("scale", {
      y: 1 + volume * 0.3,
    });
    el2.setAttribute("scale", {
      y: 3 - volume * 0.3,
    });
    el3.setAttribute("scale", {
      y: 2 + volume * 0.3,
    });
  },
});
