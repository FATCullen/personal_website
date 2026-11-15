// Tesseract rotating
// Runs ZW, YW, XY rotations

export default function createAnimation(canvas, colours) {
  const ctx = canvas.getContext('2d');
  let raf, running = false;
  let w = 0, h = 0, lastTick = 0;

  const TICK_MS = 10;

  const CAM_DISTANCE_W = 3;
  const CAM_DISTANCE_Z = 5;

  const SCALING = 5000;

  const THETA_ZW = 0.0001;
  const THETA_YW = 0.0001;
  const THETA_XY = 0.00005;

  const PHI = (1 + Math.sqrt(5)) / 2;

  let points = [];
  let projectedPoints = [];
  let connections = [];

  function getDistanceSqrd(point1, point2) {
    return (point1[0] - point2[0]) * (point1[0] - point2[0]) +
           (point1[1] - point2[1]) * (point1[1] - point2[1]) +
           (point1[2] - point2[2]) * (point1[2] - point2[2]) +
           (point1[3] - point2[3]) * (point1[3] - point2[3]);
  }

  function getScreenCoord(point) {
    // 4d projection
    const factor = 1 / ((CAM_DISTANCE_Z * (CAM_DISTANCE_W - point[3])) - point[2]);
    return [point[0] * factor * SCALING + (w * 0.5), point[1] * factor * SCALING + (h * 0.5)];
  }

  function projectPoints() {
    projectedPoints.length = 0;
    for (const point of points) projectedPoints.push(getScreenCoord(point));
  }

  function makePoints() {
    let temp = Array(4).fill(0);
    temp.fill(0.5);
    for(let i = 0; i < 16; i++) {
        points.push([...temp]);
        for (let j = 0; j < 4; j++){
            temp[j] *= -1;
            if (temp[j] < 0) break;
        }
    }
  }

  function makeConnections() {
    // Edge length in tesseract
    const EDGE_LENGTH = 1;

    for (let i = 0; i < points.length; i++) {
        for (let j = i + 1; j < points.length; j++) {
            const distance = getDistanceSqrd(points[i], points[j]);
            if (distance === EDGE_LENGTH) connections.push([i, j]);
        }
    }
  }

  function step(dt) {
    for (let point of points) {
        let z = point[2];
        let w = point[3];
        point[2] = z * Math.cos(dt * THETA_ZW) - w * Math.sin(dt * THETA_ZW);
        point[3] = z * Math.sin(dt * THETA_ZW) + w * Math.cos(dt * THETA_ZW);
        let y = point[1];
        w = point[3];
        point[1] = y * Math.cos(dt * THETA_YW) - w * Math.sin(dt * THETA_YW);
        point[3] = y * Math.sin(dt * THETA_YW) + w * Math.cos(dt * THETA_YW);
        let x = point[0];
        y = point[1];
        point[0] = x * Math.cos(dt * THETA_XY) - y * Math.sin(dt * THETA_XY);
        point[1] = x * Math.sin(dt * THETA_XY) + y * Math.cos(dt * THETA_XY);
    }
  }

  function draw() {
    ctx.fillStyle = colours.bg;
    ctx.fillRect(0, 0, w, h);
    ctx.strokeStyle = colours.fg;
    ctx.lineWidth = 2;

    projectPoints();

    for (const connection of connections) {
      const point1 = projectedPoints[connection[0]];
      const point2 = projectedPoints[connection[1]];
      ctx.beginPath();       
      ctx.moveTo(point1[0], point1[1]);    
      ctx.lineTo(point2[0], point2[1]);
      ctx.stroke();
    }
  }

  function frame(t) {
    if (!running) return;
    const dt = t - lastTick;
    if (dt > TICK_MS) {
      step(dt);
      lastTick = t;
    }
    draw();
    if (running) {
      raf = requestAnimationFrame(frame);
    }
  }

  return {
    start() {
      if (running) return;
      if (points.length === 0) makePoints();
      if (connections.length === 0) makeConnections();
      running = true;
      if (raf) cancelAnimationFrame(raf);
      raf = requestAnimationFrame((t) => {
        lastTick = t;
        frame(t);
      });
    },
    stop() {
      running = false;
      lastTick = 0;
      if (raf) cancelAnimationFrame(raf);
      raf = undefined;
    },
    resize(newW, newH) {
      w = canvas.width = newW;
      h = canvas.height = newH;
    },
  };
}