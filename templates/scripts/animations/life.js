// Conways game of life
// Uses a fairly naive approach, keeping a 2d array of 1/0 values to indicate if a cell is alive or dead
// and checking values of all a cells neighbours to determine its next state (wrapping on edges).  
// Not super efficient (ideally would only track 'live' cells) but at this scales/speeds that's not too important

export default function createAnimation(canvas, colours) {
  const ctx = canvas.getContext('2d');
  let raf, running = false;
  let w = 0, h = 0;

  const CELL_SIZE = 12;
  const INIT_ALIVE_PERCENT = 0.1;
  const TICK_MS = 1000;

  let cells = [];
  let lastTick = 0, rows = 0, cols = 0;

  function sumNeighbours(x, y) {
    let sum = 0;
    for (let dy = -1; dy <= 1; dy++) {
        for (let dx = -1; dx <= 1; dx++) {
            if (dx || dy) {
                sum += cells[(((y + dy) % rows) + rows) % rows]
                            [(((x + dx) % cols) + cols) % cols]
            }
        }
    }
    return sum;
  }

  function step(dt) {
    const next = cells.map((row) => row.slice());
    for (let y = 0; y < rows; y++) {
      for (let x = 0; x < cols; x++) {
        const sum = sumNeighbours(x, y);
        const alive = cells[y][x];
        next[y][x] = alive ? (sum === 2 || sum === 3 ? 1 : 0) : (sum === 3 ? 1 : 0);
      }
    }
    cells = next;
  }

  function draw() {
    ctx.fillStyle = colours.bg;
    ctx.fillRect(0, 0, w, h);
    ctx.fillStyle = colours.fg;
    for (let y = 0; y < rows; y++) {
      for (let x = 0; x < cols; x++) {
        if (cells[y][x] === 1) {
            ctx.fillRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }
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
      cols = Math.ceil(w / CELL_SIZE);
      rows = Math.ceil(h / CELL_SIZE);
      if (cells.length === 0) cells = 
        Array.from({ length: rows }, () => Array.from({ length: cols }, () => Math.random() < INIT_ALIVE_PERCENT ? 1 : 0));
      step();
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
      cols = Math.ceil(w / CELL_SIZE);
      rows = Math.ceil(h / CELL_SIZE);
      cells = Array.from({ length: rows }, () => Array.from({ length: cols }, () => Math.random() < INIT_ALIVE_PERCENT ? 1 : 0));
      step();
    },
  };
}