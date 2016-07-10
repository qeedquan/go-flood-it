package main

import (
	"flag"
	"fmt"
	"image"
	"image/draw"
	"math/rand"
	"os"
	"path/filepath"
	"runtime"
	"time"

	"github.com/qeedquan/go-media/sdl"
	"github.com/qeedquan/go-media/sdl/sdlgfx"
	"github.com/qeedquan/go-media/sdl/sdlimage/sdlcolor"
	"github.com/qeedquan/go-media/sdl/sdlttf"
)

const (
	SCREEN_WIDTH  = 320
	SCREEN_HEIGHT = 240

	BOARD_SIZE = 14
	BLOCK_SIZE = 16

	OFFSET_BOARD_X = 8
	OFFSET_BOARD_Y = 8
)

const (
	BLUE = iota
	GREEN
	ORANGE
	RED
	VIOLET
	YELLOW
)

func main() {
	game := NewGame(NewConfig())
	game.Execute()
}

type Config struct {
	Assets     string
	Fullscreen bool
}

func NewConfig() Config {
	conf := Config{
		Assets: filepath.Join(sdl.GetBasePath(), "assets"),
	}
	flag.StringVar(&conf.Assets, "assets", conf.Assets, "assets directory")
	flag.BoolVar(&conf.Fullscreen, "fullscreen", false, "fullscreen mode")
	flag.Parse()
	return conf
}

type Board struct {
	Step     int
	grid     [BOARD_SIZE][BOARD_SIZE]int
	oldColor int
	win      bool
}

func NewBoard() *Board {
	rand.Seed(time.Now().UnixNano())
	b := &Board{}
	b.Reset()
	return b
}

func (b *Board) generate() {
	for i := 0; i < BOARD_SIZE; i++ {
		for j := 0; j < BOARD_SIZE; j++ {
			b.grid[i][j] = rand.Intn(6)
		}
	}
}

func (b *Board) Reset() {
	b.generate()
	b.Step = 0
	b.win = false
}

func (b *Board) SetColorByCoordinate(x, y int) {
	b.SetColorByNumber(b.grid[x][y])
}

func (b *Board) SetColorByNumber(color int) bool {
	if color == b.grid[0][0] || b.IsLose() || b.IsWin() {
		return false
	}
	b.oldColor = b.grid[0][0]
	b.ChangeColor(0, 0, color)

	b.Step++
	b.win = b.checkForWin()
	return true
}

func (b *Board) ChangeColor(x, y, color int) {
	b.grid[x][y] = color
	if x-1 >= 0 && b.grid[x-1][y] == b.oldColor {
		b.ChangeColor(x-1, y, color)
	}

	if x+1 < BOARD_SIZE && b.grid[x+1][y] == b.oldColor {
		b.ChangeColor(x+1, y, color)
	}

	if y-1 >= 0 && b.grid[x][y-1] == b.oldColor {
		b.ChangeColor(x, y-1, color)
	}

	if y+1 < BOARD_SIZE && b.grid[x][y+1] == b.oldColor {
		b.ChangeColor(x, y+1, color)
	}
}

func (b *Board) IsLose() bool { return b.Step > 25 }
func (b *Board) IsWin() bool  { return b.win }

func (b *Board) checkForWin() bool {
	if b.Step > 25 {
		return false
	}
	for i := 0; i < BOARD_SIZE; i++ {
		for j := 0; j < BOARD_SIZE; j++ {
			if b.grid[i][j] != b.grid[0][0] {
				return false
			}
		}
	}
	return true
}

func (b *Board) Draw(re *sdl.Renderer) {
	for i := int32(0); i < BOARD_SIZE; i++ {
		for j := int32(0); j < BOARD_SIZE; j++ {
			r := sdl.Rect{i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE}
			c := sdl.Color{}
			switch b.grid[i][j] {
			case BLUE:
				c = sdl.Color{0x00, 0x00, 0xff, 0xff}
			case GREEN:
				c = sdl.Color{0x00, 0x80, 0x00, 0xff}
			case ORANGE:
				c = sdl.Color{0xff, 0xa5, 0x00, 0xff}
			case RED:
				c = sdl.Color{0xff, 0x00, 0x00, 0xff}
			case VIOLET:
				c = sdl.Color{0xee, 0x82, 0xee, 0xff}
			case YELLOW:
				c = sdl.Color{0xff, 0xff, 0x00, 0xff}
			}
			re.SetDrawColor(c)
			re.FillRect(&r)
		}
	}
}

type Game struct {
	window   *sdl.Window
	renderer *sdl.Renderer
	fps      sdlgfx.FPSManager
	run      bool
	board    *Board
	rgba     *image.RGBA
	surface  *sdl.Surface
	texture  *sdl.Texture
	font     *sdlttf.Font
}

func NewGame(conf Config) *Game {
	runtime.LockOSThread()

	err := sdl.Init(sdl.INIT_EVERYTHING &^ sdl.INIT_AUDIO)
	ck(err)

	err = sdlttf.Init()
	ck(err)

	wflag := sdl.WINDOW_RESIZABLE
	if conf.Fullscreen {
		wflag |= sdl.WINDOW_FULLSCREEN_DESKTOP
	}
	window, renderer, err := sdl.CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, wflag)
	ck(err)

	window.SetTitle("Flood-It!")
	renderer.SetLogicalSize(SCREEN_WIDTH, SCREEN_HEIGHT)

	surface, err := sdl.CreateRGBSurface(sdl.SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000)
	ck(err)

	texture, err := renderer.CreateTexture(sdl.PIXELFORMAT_ABGR8888, sdl.TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT)
	ck(err)

	name := filepath.Join(conf.Assets, "Vera.ttf")
	font, err := sdlttf.OpenFont(name, 11)
	ck(err)

	var fps sdlgfx.FPSManager
	fps.Init()
	fps.SetRate(60)

	return &Game{
		window:   window,
		renderer: renderer,
		fps:      fps,
		board:    NewBoard(),
		rgba:     image.NewRGBA(image.Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)),
		surface:  surface,
		texture:  texture,
		font:     font,
	}
}

func (c *Game) Reset() {
	c.board.Reset()
}

func (c *Game) Execute() {
	c.run = true
	for c.run {
		c.Event()
		c.Draw()
		c.fps.Delay()
	}
}

func (c *Game) Event() {
	for {
		ev := sdl.PollEvent()
		if ev == nil {
			break
		}
		switch ev := ev.(type) {
		case sdl.QuitEvent:
			c.run = false
		case sdl.KeyDownEvent:
			switch ev.Sym {
			case sdl.K_ESCAPE:
				c.run = false
			}
		case sdl.MouseButtonDownEvent:
			c.clicked(ev)
		}
	}
}

func (c *Game) clicked(ev sdl.MouseButtonDownEvent) {
	x, y := ev.X, ev.Y

	// click on board
	if (x < BLOCK_SIZE*BOARD_SIZE+OFFSET_BOARD_X &&
		y < BLOCK_SIZE*BOARD_SIZE+OFFSET_BOARD_X) &&
		(x > OFFSET_BOARD_X && y > OFFSET_BOARD_Y) {
		c.board.SetColorByCoordinate(
			int(x-OFFSET_BOARD_X)/BLOCK_SIZE,
			int(y-OFFSET_BOARD_Y)/BLOCK_SIZE,
		)
	}

	// click on new game button
	if (x < 240+72 && y < 180+20) && (x > 240 && y > 180) {
		c.Reset()
	}

	// click on exit button
	if (x < 240+72 && y < 212+20) && (x > 240 && y > 212) {
		c.run = false
	}

	// click on blue button
	if (x < 240+32 && y < 56+32) && (x > 240 && y > 56) {
		c.board.SetColorByNumber(0)
	}

	// click on green button
	if (x < 280+32 && y < 56+32) && (x > 280 && y > 56) {
		c.board.SetColorByNumber(1)
	}

	// click on orange button
	if (x < 240+32 && y < 96+32) && (x > 240 && y > 96) {
		c.board.SetColorByNumber(2)
	}

	// click on red button
	if (x < 280+32 && y < 96+32) && (x > 280 && y > 96) {
		c.board.SetColorByNumber(3)
	}

	// click on violet button
	if (x < 240+32 && y < 136+32) && (x > 240 && y > 136) {
		c.board.SetColorByNumber(4)
	}

	// click on yellow button
	if (x < 280+32 && y < 136+32) && (x > 280 && y > 136) {
		c.board.SetColorByNumber(5)
	}
}

func (c *Game) Draw() {
	re := c.renderer
	re.SetDrawColor(sdlcolor.Black)
	re.Clear()

	c.drawText(244, 8, "Step: %d/25", c.board.Step)

	b := c.board
	b.Draw(re)
	if b.IsLose() {
		c.drawText(250, 30, "You lose")
	}
	if b.IsWin() {
		c.drawText(253, 30, "You win")
	}

	c.drawButton("New Game", 240, 180, 72, 20, 246, 183)
	c.drawButton("Exit", 240, 212, 72, 20, 267, 215)

	c.drawRect(240, 56, 32, 32, 0, 0, 0xff)
	c.drawRect(280, 56, 32, 32, 0x00, 0x80, 0x00)
	c.drawRect(240, 96, 32, 32, 0xff, 0xa5, 0x00)
	c.drawRect(280, 96, 32, 32, 0xff, 0x00, 0x00)
	c.drawRect(240, 136, 32, 32, 0xee, 0x82, 0xee)
	c.drawRect(280, 136, 32, 32, 0xff, 0xff, 0x00)

	re.Present()
}

func (c *Game) drawButton(text string, x, y, w, h, xt, yt int32) {
	c.drawRect(x, y, w, h, 255, 255, 255)
	c.drawRect(x+1, y+1, w-2, h-2, 0, 0, 0)
	c.drawText(xt, yt, text)
}

func (c *Game) drawText(x, y int32, format string, args ...interface{}) {
	text := fmt.Sprintf(format, args...)
	r, err := c.font.RenderUTF8BlendedEx(c.surface, text, sdlcolor.White)
	ck(err)
	draw.Draw(c.rgba, c.rgba.Bounds(), c.surface, image.ZP, draw.Src)

	b, err := c.texture.Lock(nil)
	ck(err)
	for y := 0; y < int(r.H); y++ {
		for x := 0; x < int(r.W); x++ {
			i := y*c.rgba.Stride + x*4
			p := c.rgba.Pix[i:]
			b[i] = p[0]
			b[i+1] = p[1]
			b[i+2] = p[2]
			b[i+3] = p[3]
		}
	}
	c.texture.Unlock()

	c.renderer.Copy(c.texture, &r, &sdl.Rect{x, y, r.W, r.H})
}

func (c *Game) drawRect(x, y, w, h int32, r, g, b uint8) {
	re := c.renderer
	re.SetDrawColor(sdl.Color{r, g, b, 0xff})
	re.FillRect(&sdl.Rect{x, y, w, h})
}

func ck(err error) {
	if err != nil {
		sdl.LogCritical(sdl.LOG_CATEGORY_APPLICATION, "%v", err)
		sdl.ShowSimpleMessageBox(sdl.MESSAGEBOX_ERROR, "Error", err.Error(), nil)
		os.Exit(1)
	}
}
