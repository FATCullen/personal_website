// n-body sim
// Uses velocity verlet integration

export default function createAnimation(canvas, colours) {
  const ctx = canvas.getContext('2d');
  let raf, running = false;
  let w = 0, h = 0, lastTick = 0;

  const TICK_MS = 10;

  const BODY_COUNT = 4;

  const SOFTENING = 500;
  const G = 0.03;

  const SOFTENING_SQRD = SOFTENING * SOFTENING;

  let bodies = [];

  function distanceSqrd(body1, body2) {
    return (body1.x - body2.x) * (body1.x - body2.x) + (body1.y - body2.y) * (body1.y - body2.y);
  }

  function relativeAngle(origin, target) {
    return Math.atan2((target.y - origin.y), (target.x - origin.x))
  }

  function makeBody() {
    return {
        x:  Math.random() * w,
        y:  Math.random() * h,
        nextx: 0,
        nexty: 0,
        // No velocity, ensures 0 net momentum, and thus stationary centre of mass for all bodies
        dx: 0,
        dy: 0,
        r: Math.random() * 20 + 30,
    };
  }

  function verletHalf(dt) {
    let atx, aty;
    for (let body of bodies) {
        atx = 0;
        aty = 0;
        for (const other of bodies) {
            const distance = distanceSqrd(body, other);
            if (body !== other) {
                // pi = 3 :)
                const magnitude = (G * 3 * other.r * other.r) / (distance + SOFTENING_SQRD);
                const angle = relativeAngle(body, other);
                atx += magnitude * Math.cos(angle);
                aty += magnitude * Math.sin(angle);
            }
        }
        body.nextx = (body.dx + (0.5 * atx * dt)) * dt;
        body.nexty = (body.dy + (0.5 * aty * dt)) * dt;
        body.dx += 0.5 * atx * dt;
        body.dy += 0.5 * aty * dt;
    }
  }

  function step(dt) {
    verletHalf(dt);
    for (let body of bodies) {
        body.x += body.nextx;
        body.y += body.nexty;
    }
    verletHalf(dt);
  }

  function draw() {
    ctx.fillStyle = colours.bg;
    ctx.fillRect(0, 0, w, h);
    ctx.fillStyle = colours.fg;

    for (const body of bodies) {
      ctx.beginPath();
      ctx.arc(body.x, body.y, body.r, 0, 2 * Math.PI);
      ctx.fill();
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
      if (bodies.length === 0) bodies = Array.from({ length: BODY_COUNT }, makeBody);
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