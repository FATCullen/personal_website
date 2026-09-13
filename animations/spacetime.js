// Grid deformed by moving points (spacetime grid)
// This ended up being a lot more complicated than I thought it would be
// Grid modeled as splines passing through points for smooth curves,
// then points are deformed by moving deformers according to a gaussian distribution
// to avoid snapping when deformer passes close while still dropping off at further distances

export default function createAnimation(canvas, colours) {
  const ctx = canvas.getContext('2d');
  let raf, running = false;
  let w = 0, h = 0, lastTick = 0;

  const TICK_MS = 10;
  
  const GRID_LINE_COUNT = 10;
  const DEFORMER_COUNT = 4;
  const DEFORMER_SPEED = 1.4;
  const DEFORMER_RADIUS = 180;
  const DEFORMER_STRENGTH = 26;
  const DEFORMER_PEAK_RATIO = 0.15; 
  const DEFORMER_SIGMA_RATIO = 0.30; 

  let grid = [];
  let deformedGrid = [];
  let deformers = [];

  function drawSpline(points, tension = 0.24) {
    if (points.length < 2) return;

    ctx.beginPath();
    ctx.moveTo(points[0].x, points[0].y);

    if (points.length === 2) {
      ctx.lineTo(points[1].x, points[1].y);
      ctx.stroke();
      return;
    }

    for (let i = 0; i < points.length - 1; i++) {
      const p0 = points[i - 1] || points[i];
      const p1 = points[i];
      const p2 = points[i + 1];
      const p3 = points[i + 2] || p2;

      const cp1x = p1.x + (p2.x - p0.x) * tension;
      const cp1y = p1.y + (p2.y - p0.y) * tension;
      const cp2x = p2.x - (p3.x - p1.x) * tension;
      const cp2y = p2.y - (p3.y - p1.y) * tension;

      ctx.bezierCurveTo(cp1x, cp1y, cp2x, cp2y, p2.x, p2.y);
    }

    ctx.stroke();
  }

  function buildGrid() {
    const cellSize = Math.floor(h / GRID_LINE_COUNT);
    const cols = Math.ceil(w / cellSize) + 2;
    const rows = Math.ceil(h / cellSize) + 2;
    grid = [];

    for (let rowIndex = 0; rowIndex < rows; rowIndex++) {
      const row = [];
      for (let colIndex = 0; colIndex < cols; colIndex++) {
        row.push({ x: colIndex * cellSize, y: rowIndex * cellSize });
      }
      grid.push(row);
    }
  }

  function createDeformers() {
    deformers = Array.from({ length: DEFORMER_COUNT }, () => {
      const speed = (Math.random() * 1.5 + 0.8) * DEFORMER_SPEED;
      return {
        x: Math.random() * w,
        y: Math.random() * h,
        dx: (Math.random() > 0.5 ? 1 : -1) * speed * (Math.random() + 0.2),
        dy: (Math.random() > 0.5 ? 1 : -1) * speed * (Math.random() + 0.2),
        strength: -DEFORMER_STRENGTH,
        radius: DEFORMER_RADIUS + Math.random() * 80,
      };
    });
  }

  function bounce(deformer) {
    if      (deformer.x < 0) deformer.dx =  Math.abs(deformer.dx);
    else if (deformer.x > w) deformer.dx = -Math.abs(deformer.dx);
    if      (deformer.y < 0) deformer.dy =  Math.abs(deformer.dy);
    else if (deformer.y > h) deformer.dy = -Math.abs(deformer.dy);
  }

  function deformGrid() {
    deformedGrid = new Array(grid.length);

    for (let i = 0; i < grid.length; i++) {
      const sourceRow = grid[i];
      const targetRow = new Array(sourceRow.length);

      for (let j = 0; j < sourceRow.length; j++) {
        let x = sourceRow[j].x;
        let y = sourceRow[j].y;

        if (i != 0 && j != 0 && i != grid.length - 1 && j != sourceRow.length - 1) {
          for (const deformer of deformers) {
            const dx = x - deformer.x;
            const dy = y - deformer.y;
            const distSq = dx * dx + dy * dy;
            const radius = deformer.radius;
            const radiusSq = radius * radius;

            if (distSq >= radiusSq) continue; 

            const distance = Math.sqrt(distSq);

            if (distance === 0) continue; 

            const peak = radius * DEFORMER_PEAK_RATIO;
            const sigma = Math.max(1, radius * DEFORMER_SIGMA_RATIO);

            const gauss = Math.exp(-((distance - peak) * (distance - peak)) / (2 * sigma * sigma));

            const ramp = Math.min(1, distance / (peak + 1e-6));

            const influence = gauss * ramp;
            const displacement = deformer.strength * influence;

            x += (dx / distance) * displacement;
            y += (dy / distance) * displacement;
          }
        }

        targetRow[j] = { x, y };
      }

      deformedGrid[i] = targetRow;
    }
  }

  function initialize() {
    buildGrid();
    createDeformers();
    deformGrid();
  }

  function step(dt) {
    for (const deformer of deformers) {
      bounce(deformer);
      deformer.x += deformer.dx * dt * 0.03;
      deformer.y += deformer.dy * dt * 0.03;
    }

    deformGrid();
  }

  function draw() {
    ctx.fillStyle = colours.bg;
    ctx.fillRect(0, 0, w, h);
    ctx.fillStyle = colours.fg;
    ctx.strokeStyle = colours.fg;
    ctx.lineWidth = 2;

    for (const row of deformedGrid) {
        drawSpline(row);
    }
    for (let i = 0; i < deformedGrid[0].length; i++) {
        let column = deformedGrid.map((row) => row[i]);
        drawSpline(column);
    }
    for (const deformer of deformers) {
      ctx.beginPath();
      ctx.arc(deformer.x, deformer.y, 10, 0, Math.PI * 2);
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
      if (grid.length === 0) initialize();
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

      if (w > 0 && h > 0) {
        initialize();
      }
    },
  };
}