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
    var el4 = document.querySelector(".environmentDressing4");

    analyserComponent = analyserEl.components.audioanalyser;
    if (!analyserComponent.analyser) {
      return;
    }

    // 20 ~ 60 Hz
    const subBass =
      analyserComponent.levels
        .slice(10, 30)
        .reduce((prev, sum) => (sum = sum + prev), 0) /
      (30 - 10);

    // 60 ~ 250
    const bass =
      analyserComponent.levels
        .slice(30, 125)
        .reduce((prev, sum) => (sum = sum + prev), 0) /
      (125 - 30);

    //250 ~ 500
    const lowMid =
      analyserComponent.levels
        .slice(125, 250)
        .reduce((prev, sum) => (sum = sum + prev), 0) /
      (250 - 125);

    // 500 ~ 2000
    const mid =
      analyserComponent.levels
        .slice(250, 1000)
        .reduce((prev, sum) => (sum = sum + prev), 0) /
      (1000 - 250);

    // 2000 ~ 6000x (4000o)
    const highMid =
      analyserComponent.levels
        .slice(1000, 2000)
        .reduce((prev, sum) => (sum = sum + prev), 0) /
      (2000 - 1000);

    el.setAttribute("scale", {
      y: 0.5 + bass * 0.01,
    });
    el2.setAttribute("scale", {
      y: 1.0 + lowMid * 0.01,
    });
    el3.setAttribute("scale", {
      y: 1.5 + mid * 0.01,
    });
    el4.setAttribute("scale", {
      y: 2.0 + highMid * 0.01,
    });
  },
});
