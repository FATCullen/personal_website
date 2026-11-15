// 600 cell (4d polytope) rotating
// Simple 4d-2d projection, plus rotation in zw plane

export default function createAnimation(canvas, colours) {
  const ctx = canvas.getContext('2d');
  let raf, running = false;
  let w = 0, h = 0, lastTick = 0;

  const TICK_MS = 10;

  const CAM_DISTANCE_W = 3;
  const CAM_DISTANCE_Z = 5;

  const SCALING = 5000;

  const THETA = 0.0001;

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

  function getPermutations(arr) {
    const output = [];
    const tokens = [...arr];

    function generate(n) {
        if (n === 1) {
        output.push([...tokens]);
        return;
        }

        for (let i = 0; i < n; i++) {
        generate(n - 1);

        if (i < n - 1) {
            const swapIndex = n % 2 === 0 ? i : 0;
            [tokens[swapIndex], tokens[n - 1]] = [tokens[n - 1], tokens[swapIndex]];
        }
        }
    }

    generate(tokens.length);
    return output;
  }

  function getScreenCoord(point) {
    // 4d projection
    const factor = 1 / ((CAM_DISTANCE_Z * (CAM_DISTANCE_W - point[3])) - point[2]);
    return [point[0] * factor + (w * 0.5), point[1] * factor + (h * 0.5)];
  }

  function projectPoints() {
    projectedPoints.length = 0;
    for (const point of points) projectedPoints.push(getScreenCoord(point));
  }

  // Not clean, but it gets the job done;
  function makePoints() {
    points.length = 0;

    let temp = Array(4).fill(0);
    for(let i = 0; i < 4; i++) {
        temp[i] = 1;
        points.push([...temp]);
        temp[i] = -1;
        points.push([...temp]);
        temp[i] = 0;
    }
    temp.fill(0.5);
    for(let i = 0; i < 16; i++) {
        points.push([...temp]);
        for (let j = 0; j < 4; j++){
            temp[j] *= -1;
            if (temp[j] < 0) break;
        }
    }
    temp[0] = PHI / 2;
    temp[1] = 0.5;
    temp[2] = 0.5 / PHI;
    temp[3] = 0;
    for(let i = 0; i < 8; i++) {
        points.push(...getPermutations(temp).filter((_, index) => index % 2 === 0));
        for (let j = 0; j < 4; j++){
            temp[j] *= -1;
            if (temp[j] < 0) break;
        }
    }

    makeConnections();

    // Pre-scale x and y, only works because we're rotating in zw
    for (let point of points) {
        point[0] *= SCALING;
        point[1] *= SCALING;
    }
  }

  function makeConnections() {
    connections.length = 0;

    // Edge length in unit 600 cell
    const INV_PHI_SQRD = 1 / (PHI * PHI);
    // A little wiggle room, for floating point errors
    const lower = INV_PHI_SQRD - 0.05;
    const upper = INV_PHI_SQRD + 0.05;

    for (let i = 0; i < points.length; i++) {
        for (let j = i + 1; j < points.length; j++) {
            const distance = getDistanceSqrd(points[i], points[j]);
            if (distance > lower && distance < upper) connections.push([i, j]);
        }
    }
  }

  function step(dt) {
    for (let point of points) {
        let z = point[2];
        let w = point[3];
        point[2] = z * Math.cos(dt * THETA) - w * Math.sin(dt * THETA);
        point[3] = z * Math.sin(dt * THETA) + w * Math.cos(dt * THETA);
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