// Boid simulation
// Standard separation, alignment, cohesion rules, plus extra rules to limit speed and stay within bounds

export default function createAnimation(canvas, colours) {
  const ctx = canvas.getContext('2d');
  let raf, running = false;
  let w = 0, h = 0, lastTick = 0;
  
  const TICK_MS = 10;

  const BOID_COUNT = 100;
  const SPEED_LIMIT = 8;
  
  const SEPARATION_FACTOR = 0.05;
  const SEPARATION_DISTANCE = 20;

  const ALIGNMENT_FACTOR = 0.05;
  const ALIGNMENT_DISTANCE = 80; 
  
  const COHESION_FACTOR = 0.005;
  const COHESION_DISTANCE = 80;

  const SPEED_LIMIT_SQRD = SPEED_LIMIT * SPEED_LIMIT;
  const SEPARATION_DISTANCE_SQRD = SEPARATION_DISTANCE * SEPARATION_DISTANCE;
  const ALIGNMENT_DISTANCE_SQRD = ALIGNMENT_DISTANCE * ALIGNMENT_DISTANCE;
  const COHESION_DISTANCE_SQRD = COHESION_DISTANCE * COHESION_DISTANCE;

  let boids = [];

  function distanceSqrd(boid1, boid2) {
    return (boid1.x - boid2.x) * (boid1.x - boid2.x) + (boid1.y - boid2.y) * (boid1.y - boid2.y);
  }

  function makeBoid() {
    return {
      x:  Math.random() * w,
      y:  Math.random() * h,
      dx: Math.random() * (SPEED_LIMIT * 2) - SPEED_LIMIT,
      dy: Math.random() * (SPEED_LIMIT * 2) - SPEED_LIMIT,
    };
  }

  function bounce(boid) {
    if      (boid.x < 0) boid.dx =  Math.abs(boid.dx);
    else if (boid.x > w) boid.dx = -Math.abs(boid.dx);
    if      (boid.y < 0) boid.dy =  Math.abs(boid.dy);
    else if (boid.y > h) boid.dy = -Math.abs(boid.dy);
  }

  function slowDown(boid) {
    const speed = boid.dx * boid.dx + boid.dy * boid.dy;

    if (speed > SPEED_LIMIT_SQRD) {
      const reducer = SPEED_LIMIT / Math.sqrt(speed);
      boid.dx *= reducer;
      boid.dy *= reducer;
    }
  }

  function separation(boid) {
    let changeX = 0;
    let changeY = 0;

    for (const other of boids) {
      if (distanceSqrd(boid, other) < SEPARATION_DISTANCE_SQRD && boid !== other) {
        changeX += boid.x - other.x;
        changeY += boid.y - other.y;
      }
    }

    boid.dx += changeX * SEPARATION_FACTOR;
    boid.dy += changeY * SEPARATION_FACTOR;
  }

  function alignment(boid) {
    let avgX = 0;
    let avgY = 0;
    let num = 0;

    for (const other of boids) {
      if (distanceSqrd(boid, other) < ALIGNMENT_DISTANCE_SQRD && boid !== other) {
        avgX += other.dx;
        avgY += other.dy;
        num += 1;
      }
    }

    if (num) {
      avgX /= num;
      avgY /= num;

      boid.dx += (avgX - boid.dx) * ALIGNMENT_FACTOR;
      boid.dy += (avgY - boid.dy) * ALIGNMENT_FACTOR;
    }
  }

  function cohesion(boid) {
    let centreX = 0;
    let centreY = 0;
    let num = 0;

    for (const other of boids) {
      if (distanceSqrd(boid, other) < COHESION_DISTANCE_SQRD && boid !== other) {
        centreX += other.x;
        centreY += other.y;
        num += 1;
      }
    }

    if (num) {
      centreX /= num;
      centreY /= num;

      boid.dx += (centreX - boid.x) * COHESION_FACTOR;
      boid.dy += (centreY - boid.y) * COHESION_FACTOR;
    }
  }

  function step(dt) {
    for (let boid of boids) {
      bounce(boid);
      slowDown(boid);
      separation(boid);
      alignment(boid);
      cohesion(boid);

      boid.x += boid.dx * dt * 0.03;
      boid.y += boid.dy * dt * 0.03;
    }
  }

  function draw() {
    ctx.fillStyle = colours.bg;
    ctx.fillRect(0, 0, w, h);
    ctx.fillStyle = colours.fg;

    for (const boid of boids) {
      const angle = Math.atan2(boid.dy, boid.dx);
      ctx.translate(boid.x, boid.y);
      ctx.rotate(angle);
      ctx.translate(-boid.x, -boid.y);
      ctx.beginPath();
      ctx.moveTo(boid.x, boid.y);
      ctx.lineTo(boid.x - 15, boid.y + 5);
      ctx.lineTo(boid.x - 15, boid.y - 5);
      ctx.lineTo(boid.x, boid.y);
      ctx.fill();
      ctx.setTransform(1, 0, 0, 1, 0, 0);
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
      if (boids.length === 0) boids = Array.from({ length: BOID_COUNT }, makeBoid);
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