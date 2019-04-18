# Linux 中的防火墙 - 深之JohnChen的专栏 - CSDN博客

2005年12月23日 11:45:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1747标签：[linux																[internet																[networking																[server																[防火墙																[tcp](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)](https://so.csdn.net/so/search/s.do?q=防火墙&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=networking&t=blog)](https://so.csdn.net/so/search/s.do?q=internet&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux & Unix](https://blog.csdn.net/byxdaz/article/category/152883)

**Linux 中的防火墙**自 从 国 家 资 讯 高 速 公 路 （ NII） 的 推 动 以 来 ， 相继 的 TANET、 SeedNet、 HiNet的 苦 心 经 营 ， Internet几 乎 已 经 成 为 各 家 杂 志 的 热 烈话 题 ， 而 「 加 入 Internet、 利 用 Internet」 也 成 为 建 立 企 业 形 象 的 一 种 方 式 。 在 一阵 Internet风 潮 骚 动 之 下 ， 有 人 就 开 始 担 心 所 谓 的 「 网 路 安 全 」 问 题 。 因 为 Internet的 开 放 ， 而 使 得 所 有 成 员 的 受 惠 ， 但 也 因 为 他 的 开 放 ， 所 以 更 要 考 虑 这 个「 网 路 安 全 」 的 问 题 。 这 一 次 咱 们 要 谈 谈 的 话 题 就 是 因 为 这 个 原 因 而 开 始 的 — "Firewall"（ 防 火 墙 ） 就 是 基 於 这 个 「 网 路 安 全 」 考 量 之 下 的 一 个 产 物 。

　　 我 们 可 以 在 内 部 网 路 与 外 面 网 路 连 接 之 间 ， 设 置 一 个 具 有 Firewall功 能 的 机 器 ，利 用 它 来 过 滤 这 两 个 网 路 之 间 相 互 传 输 的 资 料 ， 然 後 透 过 相 关 软 体 的 设 定 来 限 制 不同 使 用 者 所 可 以 使 用 的 网 路 功 能 。 由 外 部 网 路 要 传 送 至 内 部 网 路 的 资 料 一 定 要 透 过设 置 了 Firewall这 个 机 器 来 转 送 ， 内 部 网 路 的 资 料 也 透 过 这 个 Firewall将 资 料 正 确的 传 送 位 於 外 部 网 路 上 的 目 的 地 。 身 为 Firewall的 机 器 就 成 了 网 路 间 的 转 运 站 。 因此 当 「 外 星 访 客 」 莅 临 时 ， 由 於 他 的 脚 步 仅 到 达 Firewall， 所 以 在 最 差 的 情 形 是 这个 「 Firewall机 器 被 摧 毁 了 !! 」 ， 但 是 内 部 网 路 却 可 以 逃 过 一 劫 而 「 苟 活 」 下 来， 基 於 牺 牲 小 我 完 成 大 我 的 原 则 下 ， 这 样 的 代 价 似 乎 是 值 得 的 。

　 　 虽然 这 样 的 手 续 似 乎 相 当 繁 琐 ， 但 是 也 因 为 如 此 ， 多 多 少 少 可 以 防 止 一 些 外 部 网 路 不速 之 客 的 入 侵 ， 以 确 保 内 部 网 路 的 安 全 。 另 一 方 面 也 可 以 限 制 内 部 网 路 对 外 部 网 路的 通 讯 ， 做 另 一 层 网 路 资 料 流 控 管 。 当 然 ， 为 了 这 个 安 全 问 题 ， 我 们 得 牺 牲 不 少 「便 利 」 、 「 速 度 」 上 的 享 受 ， 但 是 事 情 总 是 会 有 个 弥 补 的 方 式 的 ， 这 些 都 将 会 是 我们 所 涉 及 的 内 容 。

　 　 Firewall的 架 设 型 态 并 非 唯 一 的 ， 这 完 全 要 看 作业 系 统 以 及 Firewall软 体 的 功 能 来 看 。 上 面 的 图 形 就 是 一 个 最 简 单 的 Firewall架 构。 在 前 面 讲 了 一 些 Firewall的 字 眼 後 ， 得 和 我 们 的 主 角 "Linux"做 一 个 结 合 ， 开 始真 正 的 跳 入 我 们 的 重 点 — 「 如 何 利 用 Linux架 设 一 个 Firewall」 。

将Firewall功能加入Linux核心之中

　　 自 从 Linux 1.2.x之 後 的 Kernel编 译 设 定 中 ， 就 加 入 CONFIG_FIREWALL的 选 项 ， 由於 事 关 几 个 系 统 呼 叫 函 式 的 功 能 ， 因 此 如 果 要 将 Linux设 定 为 具 有 Firewall的 机 器， 那 麽 就 要 先 确 定 ， 所 使 用 的 Linux Kernel已 经 将 Firewall功 能 纳 入 了 !!

　　 当 然 ， 最 稳 当 的 方 式 就 是 重 新 编 译 Linux Kernel， 并 且 将 CONFIG_IP_FIREWALL设为 "Yes"。 还 有 一 点 需 要 注 意 的 就 是 「 架 设 一 个 最 基 本 的 Linux Firewall时 ， 必 须要 将 该 部 Linux Machine的 IP_Forwarding功 能 去 除 」 ， 我 们 利 用 下 面 的 图 示 来 稍 作解 释 。 所 以 在 处 理 Kernel重 新 编 译 的 工 作 执 行 "make config"时 ， 请 特 别 将 下 列 选择 项 目 仔 细 考 虑 加 入 。

1.Network Support(CONFIG_NET) [y]
2.TCP/IP networking(CONFIG_INET) [y]
3.IP forwarding/gatewaying(CONFIG_IP_FORWARD) [n]
4.IP firewalling(CONFIG_IP_FIREWALL) [y]
5.IP accounting(CONFIG_IP_ACCT) [y]
6.SLIP 或 PPP 驱 动 程 式 （ 如 果 对 外 来 连 接 是 利 用 拨 接 或 专 线 时 ）
　 Ex: PPP(Point-to-point) support(CONFIG_PPP) [y]
7.个 人 网 路 卡 驱 动 程 式 （ 用 来 连 接 内 部 区 域 网 路 时 ）
　 Ex: NE2000/NE1000 support(CONFIG_NE2000) [y]

　　 然 後 ， 接 着 执 行 「 make dep;make clean;make zImage」 等 等 的 编 译 工 作 （ 关 於详 细 的 Kernel重 新 编 译 请 读 者 自 行 参 考 前 几 期 光 碟 月 刊 内 的 详 细 说 明 ）

（备 注 ： 在 发 展 中 的 Linux 1.3.x将 加 入 另 一 种 Firewall的 功 能 ， 称 为 ip_masquerade， 他 是 将 目 前 由 外 部 软 体 来 转 送 资 料 的 方 式 改 为 由 Kernel内 部 直 接来 处 理 ， 听 起 来 相 当 不 错 的 消 息 ， 不 过 都 尚 在 发 展 测 试 阶 段 ， 敬 告 诸 位 ， 如 果 要 安全 一 点 ， 先 不 要 使 用 ， 带 运 作 稳 定 後 在 愉 快 用 之 ）

　 　 产 生 新 的 Kernel後 ， 请 将 产 生 的 zImage或 vmlinuz移 至 根 目 录 （ 或 是 其 他 启 动 目 录 ） ， 然 後如 果 你 使 用 LILO请 务 必 再 执 行 「 LILO」 一 次 ， 更 新 启 动 资 料 ， 然 後 重 新 启 动 Linux。

　 　 进 入 新 的 Linux後 ， 为 了 确 保 万 一 ， 请 观 察 /proc/net/snmp的 内 容 ， 执 行 方 式 如 下 ：

simon:/proc/net# cat snmp
1.→ Ip: Forwarding DefaultTTL InReceives InHdrErrors ....（省略）
2.→ Ip: 0 64 12 0 0 0 0 0 12 12 0 0 0 0 0 0 0 0 0
Icmp: InMsgs InErrors InDestUnreachs InTimeExcds ....（省略）
Icmp: 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
Tcp: RtoAlgorithm RtoMin RtoMax MaxConn ActiveOpens ....（省略）
Tcp: 0 0 0 0 0 0 0 0 0 0 0 0
Udp: InDatagrams NoPorts InErrors OutDatagrams
Udp: 12 0 0 12

　　 请 对 照 1.2.行 的 说 明 与 内 容 值 ， 如 果 2.的 第 一 个 值 为 0， 则 表 示 IP_Forwarding的功 能 并 未 被 包 含 於 Kernel中 。 所 以 ， 请 执 行 此 命 令 来 检 查 是 否 真 的 将 IP Forwarding的 功 能 已 经 被 关 掉 了 !!

设定网路位址

　 　 在 确 定 Linux具有 Firewall的 功 能 後 ， 接 下 来 要 做 的 就 是 设 定 网 路 的 位 址 ， 首 先 先 处 理 对 外 的 部 份， 既 然 这 台 Linux需 要 对 外 连 线 ， 那 麽 理 所 当 然 就 应 该 会 有 一 个 经 过 授 权 的 命 名 位址 、 而 且 是 唯 一 的 。 以 Internet而 言 这 个 网 路 位 址 （ IP Address） 是 世 界 唯 一 的 ，也 不 可 以 有 第 二 个 ， 否 则 会 有 打 架 的 情 形 ， 所 以 就 不 需 要 太 伤 脑 筋 了 !! 不 过 就 因为 对 外 的 位 址 一 定 要 唯 一 的 ， 所 以 在 设 定 内 部 网 路 的 位 址 （ IP Address） 时 ， 就 得考 虑 周 延 一 点 ， 否 则 如 果 有 内 部 机 器 与 外 部 网 路 位 址 相 同 时 ， Firewall机 器 将 可 能不 知 道 如 何 将 资 料 传 至 正 确 位 置 。

　 　 下 面 是 有 些 特 别 保 留 给 内 部 网 路 使 用 的 IP Address范 围 ：

10.0.0.0 - 10.255.255.255
172.16.0.0 - 172.31.255.255
192.168.0.0 - 192.168.255.255

　 　 如 果 ， 有 需 要 不 妨 将 不 存 在 於 Internet的 内 部 网 路 位 址 设 定 在 这 个 范 围 之 内 ， 这 样 才 不 会 影 响 到 Firewall Machine在 转 运 资 料 时 的 错 误 。

测试网路的线路

　　 再 来 的 步 骤 是 测 试 网 路 的 连 线 ， 请 注 意 ， 因 为 这 时 Linux是 个 Firewall Machine， 所 以 ， 内 部 网 路 上 的 机 器 无 法 直 接 由 本 身 机 器 与 外 面 连 线 ， 同 样 的 外 面网 路 的 机 器 也 无 法 直 接 连 到 内 部 网 路 的 机 器 ， 但 是 ， 他 们 都 应 该 可 以 连 到 这 台 Firewall上 来 。

　 　 因 此 ， 接 下 来 我 们 得 分 别 测 试 由 这 台 Linux Firewall Machine与 外 面 网 路 的 连 线 是 否 正 常 ， 然 後 在 测 试 由 内 部 网 路 上 的 机 器 与这 台 Linux是 否 可 以 维 持 正 常 的 连 线 情 形 。

　 　 千 万 不 要 硬 是 要 试 着 由 内部 网 路 上 的 机 器 直 接 去 与 外 面 的 某 台 机 器 进 行 连 线 ， 这 是 理 论 与 实 际 都 不 可 能 的 ，一 来 因 为 有 了 Firewall， 二 来 内 部 网 路 上 的 任 何 机 器 （ 除 了 这 台 Firewall之 外 ） 本来 就 不 属 於 外 面 网 路 的 一 员 （ 也 就 是 没 有 专 属 的 网 路 位 址 IP Address） 。

设定限制网路的功能

　　 架 建 Firewall的 目 的 是 在 於 安 全 的 考 量 。 虽 然 有 了 Firewall的 一 层 阻 挡 ， 可 以 稍作 某 程 度 的 保 护 ， 但 是 如 果 在 这 台 Firewall上 ， 开 放 所 有 网 路 的 功 能 ， 那 不 就 等 於一 个 完 全 的 公 开 ， 这 样 似 乎 还 不 太 恰 当 。 因 此 ， 既 然 要 达 到 安 全 控 管 的 目 的 所 以 就要 对 於 Firewall Machine上 所 提 供 的 网 路 程 式 稍 作 限 制 。

　 　 怎 麽 限 制 Firewall Machine上 所 提 供 的 网 路 功 能 呢 ？ 你 只 要 以 root的 身 分 修 改 "/etc/inet.conf"这 个 档 案 。 将 你 不 准 备 直 接 使 用 於 Firewall上 的 网 路 功 能 加 上 注解 符 号 。 一 般 来 说 是 保 存 档 案 传 输 、 终 端 模 拟 的 功 能 ， 而 将 与 系 统 资 讯 相 关 的 程 式给 关 闭 （ systat、 bootp、 fingerd....） 避 免 他 人 使 用 这 些 程 式 获 取 该 机 器 的 现 状资 讯 。

设定Proxy Server

　 　 因 为 以 这 样 架 设 起 来 的 Firewall环境 下 ， 所 有 与 外 部 网 路 相 关 的 资 料 一 定 得 透 过 这 台 Firewall（ 当 然 也 可 能 不 只 一 台） 去 连 接 。 这 样 子 ， 如 果 要 Telnet到 外 面 网 路 ， 那 就 得 先 Telnet到 Firewall Machine， 然 後 在 由 此 Telnet到 外 面 ， 也 就 是 说 「 凡 事 都 得 假 手 他 人 」 ， 那 想 想 ，如 果 内 部 有 人 想 玩 玩 WWW那 不 就 无 解 的 嘛 ？ " Proxy Server"就 是 来 协 助 此 种 问 题 的一 个 答 案 ， 下 次 我 们 就 继 续 来 说 明 ， 如 何 在 Linux Firewall Machine上 ， 建 立 SOCKS 4.2这 个 Proxy Server。

　 　 如 何 在 内 部 网 路 的 机 器 执 行 Linux、 Trumpet、 Netscape来 透 过 这 个 架 好 的 Linux Firewall对 外 连 线 !!!。 这 样 子 ， 先 撇开 安 全 性 不 提 ， 我 们 就 可 以 利 用 这 个 方 式 ， 让 好 几 台 机 器 ， 同 时 使 用 一 个 Internet IP Address， 连 到 国 际 网 路 上 去 玩 耍 啦 !!!


