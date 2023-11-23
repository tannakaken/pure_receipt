# レシート小説

[ATOM Thermal Printer Kit](https://shop.m5stack.com/products/atom-thermal-printer-kit)を使って、レシート小説を印刷する。

[Dual-Button](https://docs.m5stack.com/en/unit/dual_button)を接続し、
赤を押すと10円レシート、青を押すと100円レシートが出るようになっている。

10円を払うと10円のレシートがもらえ、100円を払うと100円のレシートがもらえる、
**純粋レシート** である。

## 実装

ATOM Thermal Printer Kit の[中国語のマニュアル](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/atombase/atom_pritner/ATOM_PRINTER_CMD_v1.06.pdf)の7.1によれば、
日本語印刷もできるはずなのだが、
頑張ってもできなかったので、
全て画像にして印刷することにした。
このせいで、印刷が非常にゆっくりになってしまったが、現場では皆がゆっくり印刷される様を見ていたのが少し面白かったので、怪我の功名である。

印刷の元データのは Google Document を使って作成した[PDF](./receipt_novel.pdf)である。

これを ImageMagick によって、印刷したいところだけ切り取った。
その処理は[シェルスクリプト](./image.sh)になっている。

これにより、グレイスケールのbmp（macではプレビューで見ることもできない）になるので、これをヘッダーファイルに変換する。
それには、[このサイト](https://m5stack.lang-ship.com/tools/image2data/?format=1bit_2&_ga=2.254427854.756178971.1700724679-1544476008.1698913618&_gl=1*147xt5t*_ga*MTU0NDQ3NjAwOC4xNjk4OTEzNjE4*_ga_KFS82DRR35*MTcwMDcyNDY3OC4xMy4wLjE3MDA3MjQ2NzguMC4wLjA.*_ga_4XK8FG0C32*MTcwMDcyNDY3OS4xMi4wLjE3MDA3MjQ2NzkuMC4wLjA.)を使わせてもらった。
実際にはこのサイトで生成したものを、型定義を少し手で書き換えて使っている。

しかしこのままでは、ランダムなレシート小説を全て格納しようとするとDRAMが溢れてしまう。
そこで、[pythonスクリプt](./src/compression.py)で、同じ値が連続しているときに
「`{値, 連続出現回数}`」という構造体に書き換えることで、データを圧縮する。
印刷時は連続出現回数分同じ値をシリアル通信に流すことで、印刷している。

同じ値は`uint8_t`に収まらない回数で連続出現するが、
ほとんどは`uint8_t`に収まる回数なので回数を`uint16_t`で表現するとメモリの無駄が発生すると考えた。
そこで出現回数は`uint8_t`として、それ以上は構造体の繰り返しで表現することにした。

またおまけ機能として、m5atomのボタンを押すと、QRコードと[パターン](./pattern.png)が印刷される。
このQRコードは[ARサイト](https://github.com/tannakaken/ar-kumajirou)を指し示し、
そのサイトを開いてこのパターンを見ると、クマジロウ（我が家のぬいぐるみ）が踊る。
さらにクマジロウをタップすると、逆立ちする。

## 参考

以下のサイトを大いに参考にさせてもらいました。

- https://lang-ship.com/blog/work/atom-printer-k118/
- https://inajob.github.io/iroiro-review/post/arduino%E3%81%A7%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%81%A7%E3%81%8D%E3%82%8Bwifi%E3%81%AB%E3%82%82%E5%AF%BE%E5%BF%9C%E3%81%97%E3%81%9F%E3%83%AC%E3%82%B7%E3%83%BC%E3%83%88%E3%83%97%E3%83%AA%E3%83%B3%E3%82%BFatom-printer/