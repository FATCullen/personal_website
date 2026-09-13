// Net of moving particles
// Recreation of effect I used on original website
// Shockingly simple

export default function createAnimation(canvas, colours) {
  const ctx = canvas.getContext('2d');
  let raf, running = false;
  let w = 0, h = 0, lastTick = 0;

  const TICK_MS = 10;

  const POINT_SPEED = 0.4;
  const CONNECTION_START = 10000;
  const CONNECTION_MAX = 1000;

  const POINT_COUNT = 200;
  let points = [];

  function bounce(point) {
    if      (point.x < 0) point.dx =  Math.abs(point.dx);
    else if (point.x > w) point.dx = -Math.abs(point.dx);
    if      (point.y < 0) point.dy =  Math.abs(point.dy);
    else if (point.y > h) point.dy = -Math.abs(point.dy);
  }

  function distanceSqrd(point1, point2) {
    return (point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y);
  }

  function makePoint() {
    return {
          x: Math.random() * w,
          y: Math.random() * h,
          dx: (Math.random() + 0.2) * POINT_SPEED * (Math.random() > 0.5 ? 1 : -1),
          dy: (Math.random() + 0.2) * POINT_SPEED * (Math.random() > 0.5 ? 1 : -1),
    };
  }

  function step(dt) {
    for (let point of points) {
        bounce(point);
        point.x += point.dx * dt * 0.03;
        point.y += point.dy * dt * 0.03;
    }
  }

  function draw() {
    ctx.fillStyle = colours.bg;
    ctx.fillRect(0, 0, w, h);
    ctx.fillStyle = colours.fg;

    for (const point of points) {
      ctx.beginPath();
      ctx.arc(point.x, point.y, 3, 0, 2 * Math.PI);
      ctx.fill();
    }

    ctx.lineWidth = 1;

    for (let i = 0; i < points.length; i++) {
        for (let j = i + 1; j < points.length; j++) {
            const point1 = points[i];
            const point2 = points[j];

            let dSqrd = distanceSqrd(point1, point2);

            if (dSqrd > CONNECTION_START) continue;
            else if (dSqrd < CONNECTION_MAX) ctx.strokeStyle = colours.fg;
            else {
                const opacity = 1 - ((dSqrd - CONNECTION_MAX) / (CONNECTION_START - CONNECTION_MAX));
                const alphaHex = Math.floor(opacity * 255).toString(16).padStart(2, '0');
                ctx.strokeStyle = colours.fg + alphaHex;
            }
            ctx.beginPath();       
            ctx.moveTo(point1.x, point1.y);    
            ctx.lineTo(point2.x, point2.y);
            ctx.stroke();
        }
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
      if (points.length === 0) points = Array.from({ length: POINT_COUNT }, makePoint);
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