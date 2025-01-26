# RGB LED Library

[**English**](README.md) | **简体中文**



## 目录

- [背景和介绍](#背景和介绍)
- [文档](#文档)
- [如何使用](#如何使用)
  - [RGB](#rgb)
  - [RGBLEDControllerBase](#rgbledcontrollerbase)
    - [介绍](#介绍)
    - [使用自己的单片机控制 WS2812](#使用自己的单片机控制-ws2812)
    - [控制更多的 RGB LED 硬件](#控制更多的-rgb-led-硬件)
  - [RGBActionBase](#rgbactionbase)
    - [介绍](#介绍-1)
    - [定制自己的灯效动画](#定制自己的灯效动画)
  - [RGBPlayer](#rgbplayer)



## 背景和介绍

今年圣诞节前，我和我的舍友决定制作一个圣诞树摆件，作为礼物送给他人。我们使用两百余个 WS2812 RGB LED 灯珠来实现多种灯效动画，使用 STM32 作为主控，并使用 Arduino IDE 作为开发平台。

我们很快完成了任务。但当我浏览我写的代码的时候，我开始考虑提高它的可扩展性和可移植性，从而使我下次想在其它开发平台上使用其它单片机控制其它型号的 RGB LED 实现更多灯效时，我只需要关注新的部分，而无需重写整个项目。于是我开始着手将项目中硬件无关代码分离出来，建立了这个仓库。

本库由 `RGB` 类，控制 RGB LED 硬件的 `RGBLEDControllerBase` 类（和它的子类），描述灯效动画的 `RGBActionBase` 类（和它的子类），以及 `RGBPlayer` 类，后者的作用是在 `RGBLEDControllerBase` 指向的 LED 硬件上播放 `RGBActionBase` 描述的灯效动画。

本库被期望实现可移植、可扩展。它应当能够在任何支持 C++11-or-later 的单片机和开发平台上使用，并支持更多的 RGB LED 硬件和灯效动画。实现这个目标还需要更多工作。



## 文档

*TODO*



## 如何使用

### RGB

描述一个对象的颜色的类，支持以下多种运算：

```c++
RGB col1, col2, col3;

col1 = 0xFF00FF;
col1 = "#FF00FF";
col1 = {0xFF, 0x00, 0xFF};
col2 = ~col1;

col3 = col1 & col2;
col3 = col1 | col2;

col3 = 0.5*col1 + 0.5*col2;
col3 = 0.5*col1 - 0.5*col2;
```



### RGBLEDControllerBase

#### 介绍

这个虚基类声明了控制 RGB LED 硬件的接口，它的基本使用方法如下（假设 `MyController` 是用户根据实际硬件情况定义的子类）：

```c++
const unsigned LED_COUNT = 50;

int main() {
    RGBLEDControllerBase *c = new MyController(LED_COUNT, /*other neccessary args*/);
    
    RGB *leds = c->getLEDs();
    for (int i = 0; i < LED_COUNT; ++i) {
        if (i & 1)
            leds[i] = 0xFF0000;
        else
            leds[i] = 0x00FF00;
    }
    c->show();
    
    delete c;
    
    return 0;
}

```

`RGB *getLEDs()` 方法会返回一个 `RGB` 类型的指针，指向一个表明 RGB LED 组状态的数组，数组的长度在构造函数中已然确定。用户可以更改该数组，并在更改完成后通过执行 `void show()` 方法使更改生效。在本例中，当执行完 `c->show();` 语句后，被控制的 50 个 RGB LED 灯将会变成红、绿相间的样子。

此外，该虚基类还声明/定义了 `getBrightness/setBrightness`, `enable/disable/isEnabled` 等方法，但它们能否发挥作用，取决于用户对子类的定义。

#### 使用自己的单片机控制 WS2812

`WS2812Base` 是库中已经定义的虚基类，继承自 `RGBLEDControllerBase`，它描述了对 WS2812 型号的 RGB LED 的控制逻辑。想要在自己的单片机和开发平台上使用它，用户应当定义它的子类（假设是 `MyWS2812`）并重写 `void write_bits(bool *bits, size_t count)` 和 `void delay_us(uint32_t us)` 两个纯虚函数。

`void write_bits(bool *bits, size_t count)` 用来向硬件发送比特信息。根据 WS2812 的文档，该硬件通过一根数据线控制，0 和 1 分别对应两种占空比不同的脉冲信号。用户可以根据自己单片机和开发平台实际提供的 API，考虑产生这两种脉冲信号的方式。比如在以下的示例中，我通过 SPI 协议的 MOSI（or PICO）线，通过分别发送 0xC0 和 0xF8 来对应两种脉冲信号。

`void delay_us(uint32_t us)` 用来实现微秒级延迟。这个函数在基类中被声明为纯虚函数，这是因为不同单片机和开发平台上实现延迟的方法不尽相同。值得一提的是，这个函数只有在重置 WS2812 的时候才会被使用，而根据文档，重置它只需要大于 280us 的延迟即可；所以这个函数并不需要被精确实现，即使它的实际延迟时间长于传入参数所要求的，也可以接受。

以下是一个在 Arduino IDE 平台下的示例：

```c++
#include "src/RGBLEDController/WS2812Base.h"

#include <SPI.h>

class MyWS2812 : public WS2812Base {
public:
	MyWS2812(unsigned ledCount, uint8_t spi_mosi, uint8_t spi_miso, uint8_t spi_sck);
	~MyWS2812();

	MyWS2812(const MyWS2812& ws2812) = delete;
	MyWS2812& operator=(const MyWS2812& ws2812) = delete;

	void begin();
	void end();

private:
	void write_bits(bool *bits, size_t count) override;
	void delay_us(uint32_t us) override;

private:
	uint8_t		m_spi_mosi, m_spi_miso, m_spi_sck;
	SPIClass	*m_spi;
}; // class MyWS2812

MyWS2812::MyWS2812(unsigned ledCount, uint8_t spi_mosi, uint8_t spi_miso, uint8_t spi_sck)
	: WS2812Base(ledCount),
	  m_spi_mosi(spi_mosi), m_spi_miso(spi_miso), m_spi_sck(spi_sck)
{
	m_spi = new SPIClass(spi_mosi, spi_miso, spi_sck);

	begin();
	enable();
}

MyWS2812::~MyWS2812() {
	end();
	delete m_spi;
}

void MyWS2812::begin() {
	m_spi->begin();
	m_spi->beginTransaction(SPISettings(8e6, MSBFIRST, SPI_MODE1));
}

void MyWS2812::end() {
	m_spi->endTransaction();
	m_spi->end();
}

void MyWS2812::write_bits(bool *bits, size_t count) {
	uint8_t buffer[24] = {0};

	for (size_t i = 0; i < count; ++i) {
		buffer[i] = bits[i] ? 0xF8 : 0xC0;
	}

	m_spi->transfer(buffer, count);
}

void MyWS2812::delay_us(uint32_t us) {
	delayMicroseconds(us);
}
```

#### 控制更多的 RGB LED 硬件

用户需要继承 `RGBLEDControllerBase` 并重写 `void show()` 。库中 `WS2812Base` 的写法或可作为示例。

如果用户完成对更多型号硬件的控制，并愿意将代码中硬件无关部分分离出来并向本仓库提交 PR，我将不胜感激。



### RGBActionBase

#### 介绍

描述灯效动画的虚基类，它的基本使用方法如下（假设 `MyAction` 是用户为定制动画而定义的子类）：

```c++
const unsigned LED_COUNT = 50;

int main() {
     RGBActionBase *action = new MyAction(LED_COUNT, {0xFF0000, 0x00FF00, 0x0000FF}, /*other necessary args*/);
    
    RGB *leds = new RGB[LED_COUNT];
    int *args = new int[action->argsCount()];
    
    action->act(nullptr, args);
    while (true) {
        int st = action->act(leds, args);
        if (st == -1) {
            break;
        }
        
        for (int i = 0; i < LED_COUNT; ++i)
            std::cout << leds[i].r << ' ' << leds[i].g << ' ' << leds[i].b << '\n';
        std::cout << std::endl;
        
        /*do something else*/
    }
    
    delete action;
    delete[] leds;
    delete[] args;
    
    return 0;
}
```

构造函数的前两个参数分别代表灯效涉及的 RGB LED 数量和初始颜色；本例中第二个实参的长度小于 `LED_COUNT`，那么它将会被循环扩展成：`{0xFF0000, 0x00FF00, 0x0000FF, 0xFF0000, 0x00FF00, 0x0000FF, ...}`；第二个参数也可以传入 `{}`，表示沿用 RGB LED 的当前颜色作为动画的初始颜色（如果子类支持）。

`int act(RGB *leds, int *args)` 每执行一次会对传入的 `leds` 数组进行一次变换，当它返回 -1 时表示动画已经播放完毕；特别的，如果第一个参数传入 `nullptr` 则表示重置动画播放状态。它的第二个参数是一个 `int` 类型的数组，用于传入一系列变量从而在多次执行中保持状态，这个数组所需的大小由 `unsigned argsCount()` 返回。

这个类的使用方法看上去似乎有些奇怪，我将解释这么做的原因：

单片机通常是单任务的，但它同时做很多事情。所以我们并不希望执行一次函数完成整个动画播放，而希望每执行一次动画仅播放一帧，这样就可以在播放动画的同时响应其它事件（如本例中 `/*do something else*/`）。同时，这么做还可以通过在循环中添加定时语句来方便地改变动画速度。

而为了实现这一点，需要维护一些变量来在多次调用时保持状态。最开始，我使用实例的成员变量来保存这些信息，这是最很方便的。但如果我想同时在两个 `RGB` 数组上播放完全相同的动画，那为了避免冲突，我将不得不实例化两个完全相同的 `MyAction` 对象。所以我决定这些变量由调用者提供——虽然这也算不上什么好办法。也就是说，我们可以这么干：

```c++
const unsigned LED_COUNT = 50;

int main() {
     RGBActionBase *action = new MyAction(LED_COUNT, {0xFF0000, 0x00FF00, 0x0000FF}, /*other necessary args*/);
    
    RGB *leds1 = new RGB[LED_COUNT];
    RGB *leds2 = new RGB[LED_COUNT];
    int *args1 = new int[action->argsCount()];
    int *args2 = new int[action->argsCount()];
    
    action->act(nullptr, args1);
    action->act(nullptr, args2);
    
    int st = 0;
    while (st >= 0) {
        st = action->act(leds1, args1);
        st = action->act(leds2, args2);
    }
    
    delete action;
    delete[] leds1;
    delete[] leds2;
    delete[] args1;
    delete[] args2;
    
    return 0;
}
```

#### 定制自己的灯效动画

用户需继承 `RGBActionBase` 类并重写 `int act(RGB *leds, int *args)` 和 `unsigned argsCount()`，同时注意基类的成员变量 `bool isHeritance`——如果它为 `true`，则说明构造函数中的初始颜色一项传入了 `{}`，表示沿用 RGB LED 的当前颜色作为动画的初始颜色，这一点需要在重写 `act` 时额外注意。

`RGBActionSolid` 是库中最简单的动画，或可作为示例。



### RGBPlayer

控制播放。它的基本使用方法如下：

```c++
const RGBActionSeries actions = {
    // 0~2
    new RGBActionSolid(56, {"#ff0000"}, 50),
    new RGBActionSolid(56, {"#00ff00"}, 50),
    new RGBActionSolid(56, {"#0000ff"}, 50),
    // 3~6
    new RGBActionFading(56, {"#ff0000"}, {0, 100, 5}, 1),
    new RGBActionFading(56, {"#ff0000"}, {100, 0, 5}, 1),
    new RGBActionFading(56, {"#0000ff"}, {0, 100, 5}, 1),
    new RGBActionFading(56, {"#0000ff"}, {100, 0, 5}, 1),
    // 7~9
    new RGBActionStreaming(56, {"#ff0000"}, 1, false, false, false),
    new RGBActionStreaming(56, {"#00ff00"}, 1, false, false, false),
    new RGBActionStreaming(56, {"#0000ff"}, 1, false, false, false),
}

MyWS2812 ws2812(112, /*other neccessary args*/);

RGBPlayer player0(&ws2812, 0, &actions, {
    {0, 1, 2},
    {3, 4, 5, 6},
    {7, 8, 9},
});

RGBPlayer player1(&ws2812, 56, &actions, {
    {0, 1, 2},
    {3, 4, 5, 6},
    {7, 8, 9},
});

int main() {
    while (true) {
        player0.play();
        player1.play();
        
        if (/*Pause button pressed*/) {
            player0.pause();
            player1.pause();
        }
        if (/*Resume button pressed*/) {
            player0.resume();
            player1.resume();
        }
        if (/*Next button pressed*/) {
            player0.next();
            player1.next();
        }
        if (/*Stop button pressed*/) {
            player0.stop();
            player1.stop();
            break;
        }
    }
    
    for (auto action : actions) {
        delete action;
    }
    
    return 0;
}
```

先看一下它的构造函数：

```c++
RGBPlayer player1(&ws2812, 56, &actions, {
    {0, 1, 2},
    {3, 4, 5, 6},
    {7, 8, 9},
});
```

`player1` 在 `ws2812` 控制的 RGB LED 组的第 56~111 个灯珠上播放动画，一共有 3 段动画，第一段动画由 `actions[0]`, `actions[1]` 和 `actions[2]` 组成，以此类推。

本例中其它方法顾名思义。注意到，这里的 `int play()` 和 `RGBActionBase` 类的 `int act(RGB*, int*)` 一样，都是在循环中调用，每调用一次播放一帧。