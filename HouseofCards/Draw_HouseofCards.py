import copy
from graphviz import Digraph
class dat_que(object):

    def init(self, dir, poi): 
        self.dir = dir # dir是方向, -1代表扑克牌朝下， 0代表扑克牌平放，1代表扑克牌向上
        self.poi = poi # 构成峰和谷的最上面8张纸牌

que = [dat_que() for _ in range(9)] # 构成峰和谷的最上面8张牌

class data_card(object):
    
    def init(self, col, dig):
        self.col = col # col是颜色
        self.dig = dig # dig是分值

card = [data_card() for _ in range(30)] # 纸牌顺序

order = ["" for _ in range(30)] # 记录每个回合出手的人的名字

INF = 1000
n = 0 # n为最大分值
node_count = 0
dot = Digraph(comment='Search Graph')
pruning = True 
# pruning = False
Draw = True
# Draw = False

def calc_hold_card(Axel_hold, Birgit_hold):
    """
    一轮结束时，计算分数
    Args:
        Axel_hold : Axel的握牌
        Birgit_hold : Birgit的握牌
    Return:
        返回输赢分数
    """ 
    s = 0
    # 若A还有握牌，则比较牌色与A的颜色．决定其分数是加入或减去握牌分数
    if Axel_hold != -1:
        if card[Axel_hold].col == 'R':
            s += card[Axel_hold].dig
        else:
            s -= card[Axel_hold].dig
    # 若B还有握牌，则比较牌色与B的颜色．决定其分数是加入或减去握牌分数
    if Birgit_hold != -1: 
        if card[Birgit_hold].col == 'B':
            s -= card[Birgit_hold].dig
        else:
            s += card[Birgit_hold].dig
    return s

def update(fo, calc, fson) : 
    """
        调整参数 f[o] = max (calc - f[son])
        注意： fson可能是非法操作，须要剪去
    """
    if fson != -INF:
        return max(fo, calc - fson)
    return fo

def calc_digit(pha, pl, p2, p3):
    """
    Args:
        pha : 玩家
        p1, p2, p3 : 构成三角形的纸牌
    Return:
        若pha的颜色和p1, p2, p3的多数颜色相同，则返回这三张纸牌的数字之和
        否则返回数字和的负数
    """
    # 红色和黑色纸牌计数
    counr = counb = 0
    # 计算构成三角形的三张纸牌的数字总和 dig
    dig = card[pl].dig + card[p2].dig + card[p3].dig
    if card[pl].col == 'B':
        counb+= 1
    else:
        counr+= 1
    if card[p2].col == 'B':
        counb+= 1
    else:
        counr+= 1
    if card[p3].col == 'B':
        counb+= 1
    else:
        counr+= 1
    # 若玩家的颜色和多数纸牌的颜色相同，则返回dig，否则返回-dig
    if counr > 1:
        if order[pha] == "Axel":
            return dig
        else:
            return -dig
    else:
        if order[pha] == "Birgit":
            return dig
        else:
            return -dig

def dfs(pha, Axel_hold, Birgit_hold, fo_calc, father_node):
    """
    Args:
        pha         : 第pha回合
        Axel_hold   ：A的握牌
        Birgit_hold ：B的握牌
        fo_calc     ：传递参数, 
                f[o] = max (calc - f[son])
                这里传递的是,fo - calc
    Return:
        从当前状态出发计算走方玩家的得分
    """
    global pruning
    global node_count
    global dot

    node_count += 1
    son_node = node_count
    # dot.node(str(son_node), str(pha) + ',' + str(Axel_hold) + ',' + str(Birgit_hold) + ',' + str(fo_calc))
    dot.node(str(son_node), "")
    dot.edge(str(father_node), str(son_node), constraint='true')

    # 若一轮游戏结束，则计算得分s
    if pha > 2 * n:
        s = calc_hold_card(Axel_hold, Birgit_hold)
        # 若当前回合是A走，则返回s，否则返回s的负数
        if order[pha] == "Axel":
            return s
        else:
            return -s
    # 预保留que，以便回溯恢复“握牌”操作
    temp_que = copy.deepcopy(que)
    # 当前回合得分初始化
    fo = -INF
    # 对手得分初始化
    fson = INF
    # 当前操作得分
    calc = 0
    # 当前回合是A走牌且暂无握牌，则递归计算下一回合B的最大得分（A握住第pha张牌)
    if order[pha] == "Axel" and Axel_hold == -1: 
        fson = dfs(pha + 1, pha, Birgit_hold, fo, son_node)
    # 当前回合是B走牌且暂无握牌，则递归计算下一回合B的最大得分（B握住第pha张牌)
    elif order[pha] == "Birgit" and Birgit_hold == -1:
        fson = dfs(pha + 1, Axel_hold, pha, fo, son_node) 
    # fson是对手取得的最大分数，因而是负值
    fo = update(fo, calc, fson) 
    # 剪枝
    if pruning:
        if fo_calc >= -fo: 
            return fo
    # 卡牌最多13张,故循环2~6
    # Floor操作
    for i in range(2, 7):
        # 若第i,i+1张牌构成peak，则计算第i,i+1张牌和玩家pha的三角形分数
        if que[i].dir == -1 and que[i + 1].dir == 1: 
            calc = calc_digit(pha, que[i].poi, que[i + 1].poi, pha)
            que[i].dir = que[i + 1].dir = 0
            que[i].poi = pha
            que[i + 1].poi = -1
            # 递归计算下一回合B的最大得分
            fson = dfs(pha + 1, Axel_hold, Birgit_hold, fo - calc, son_node)
            # 调整参数
            fo = update(fo, calc, fson)
            que[i].dir = temp_que[i].dir;que[i].poi = temp_que[i].poi
            que[i + 1].dir = temp_que[i + 1].dir;que[i + 1].poi = temp_que[i + 1].poi
            # 剪枝
            if pruning:
                if fo_calc >= -fo:
                    return fo 

            # 若当前玩家A有“握牌”
            if order[pha] == "Axel" and Axel_hold != -1:
                # 计算第i,i+1张牌和A握牌的三角形分数
                calc = calc_digit(pha, que[i].poi, que[i + 1].poi, Axel_hold) 
                # 第pha张牌放入valley之间构成floor
                que[i].dir = que[i + 1].dir = 0 
                que[i].poi = Axel_hold
                que[i + 1].poi = -1
                # 递归计算下一回合B的最大得分(A握住第pha张牌)
                fson = dfs(pha + 1, pha, Birgit_hold, fo - calc, son_node) 
                # 第pha张牌放入valley之间构成floor
                fo = update(fo, calc, fson)
                que[i].dir = temp_que[i].dir;que[i].poi = temp_que[i].poi
                que[i + 1].dir = temp_que[i + 1].dir;que[i + 1].poi = temp_que[i + 1].poi
            
            elif order[pha] == "Birgit" and Birgit_hold != -1:
                calc = calc_digit(pha, que[i].poi, que[i + 1].poi, Birgit_hold)
                que[i].dir = que[i + 1].dir = 0
                que[i].poi = Birgit_hold
                que[i + 1].poi = -1
                # 递归计算下一回合B的最大得分(A握住第pha张牌)
                fson = dfs(pha + 1, Axel_hold, pha, fo - calc, son_node)
                # 调整参数
                fo = update(fo, calc, fson)
                que[i].dir = temp_que[i].dir;que[i].poi = temp_que[i].poi
                que[i + 1].dir = temp_que[i + 1].dir;que[i + 1].poi = temp_que[i + 1].poi
            # 恢复递归之前的que
            que[i].dir = temp_que[i].dir;que[i].poi = temp_que[i].poi
            que[i + 1].dir = temp_que[i + 1].dir;que[i + 1].poi = temp_que[i + 1].poi
            # 剪枝
            if pruning:
                if fo_calc >= -fo: 
                    return fo
    
    # Peak操作
    for i in range(2, 7): 
        # 若第i,i+1个位置处于平放 PS:que.poi =-1 不能当peak的左端
        if que[i].dir == 0 and que[i + 1].dir == 0 and que[i].poi != -1 and que[i + 1].poi == -1:
            # 若当前玩家A有"握牌"
            if order[pha] == "Axel"and Axel_hold != -1:
                # 计算第i个位置的牌,i+1张牌和A握牌的三角形分数
                calc = calc_digit(pha, que[i].poi, Axel_hold, pha)
                # A的握牌放入第i + 1个位置，走牌放入第i个位置，形成peak
                que[i].dir = 1
                que[i + 1].dir = -1
                que[i].poi = pha
                que[i + 1].poi = Axel_hold
                # 递归计算下一回合B的最大得分(A的握牌为空)
                fson = dfs(pha + 1, -1, Birgit_hold, fo - calc, son_node)
                fo = update(fo, calc, fson)
                # 恢复递归前的值
                que[i].dir = temp_que[i].dir;que[i].poi = temp_que[i].poi
                que[i + 1].dir = temp_que[i + 1].dir;que[i + 1].poi = temp_que[i + 1].poi
                # 剪枝
                if pruning:
                    if fo_calc >= -fo:
                        return fo
                # 计算第i, A握牌和走牌的三角形分数
                calc = calc_digit(pha, que[i].poi, Axel_hold, pha)
                # A的握牌放入第i个位置，走牌放入第i + 1个位置，形成peak
                que[i].dir = 1
                que[i + 1].dir = -1
                que[i].poi = Axel_hold
                que[i + 1].poi = pha
                # 递归计算下一回合B的最大得分(A的握牌为空)
                fson = dfs(pha + 1, -1, Birgit_hold, fo - calc, son_node)
                # 调整参数
                fo = update(fo, calc, fson)
                # 恢复递归前的que
                que[i].dir = temp_que[i].dir;que[i].poi = temp_que[i].poi
                que[i + 1].dir = temp_que[i + 1].dir;que[i + 1].poi = temp_que[i + 1].poi
                # 剪枝
                if pruning:
                    if fo_calc >= -fo:
                        return fo
            # 若当前玩家为B且有握牌
            elif order[pha] == "Birgit" and Birgit_hold != -1:
                # 计算第i个位置的牌、B的握牌和走牌的三角形分数
                calc = calc_digit(pha, que[i].poi, Birgit_hold, pha)
                # B的握牌放入第i + 1个位置，走牌放入第i个位置，形成peak
                que[i].dir = 1
                que[i + 1].dir = -1
                que[i].poi = pha
                que[i + 1].poi = Birgit_hold
                # 递归计算下一回合A的最大得分(B的握牌为空)
                fson = dfs(pha + 1, Axel_hold, -1, fo - calc, son_node)
                # 调整参数
                fo = update(fo, calc, fson)
                # 恢复递归前的que
                que[i].dir = temp_que[i].dir;que[i].poi = temp_que[i].poi
                que[i + 1].dir = temp_que[i + 1].dir;que[i + 1].poi = temp_que[i + 1].poi
                # 剪枝
                if pruning:
                    if fo_calc >= -fo:
                        return fo
                # 计算第i个位置的牌, B握牌和走牌的三角形分数
                calc = calc_digit(pha, que[i].poi, Birgit_hold, pha)
                # B 的握牌放入第i个位置，走牌放入第i + 1个位置，形成peak
                que[i].poi = Birgit_hold
                que[i + 1].poi = pha
                # 递归计算下一回合A的最大得分(B的握牌为空)
                fson = dfs(pha + 1, Axel_hold, -1, fo - calc, son_node)
                fo = update(fo, calc, fson)
                # 恢复递归前的que
                que[i].dir = temp_que[i].dir;que[i].poi = temp_que[i].poi
                que[i + 1].dir = temp_que[i + 1].dir;que[i + 1].poi = temp_que[i + 1].poi
                # 剪枝
                if pruning:
                    if fo_calc >= -fo:
                        return fo
    return fo

def main():
    global pruning
    global node_count
    global dot
    t = 0
    while True:
        t += 1
        # 反复输入姓名，直至输入'End'位置
        name = input()
        if name == "End":
            break
        # 输入最大分值
        global n
        n = input()
        n = int(n)
        # 依次输入 2 * n 张纸牌的分值和颜色
        _str = input().split(' ')
        for i, z in enumerate(_str):
            i += 1
            _len = len(z)
            if _len == 0:
                continue
            if _len == 2:
                card[i].col = z[1]
                card[i].dig = int(z[0])
            else:
                card[i].col = z[2]
                card[i].dig = int(z[0]) * 10 + int(z[1])
        # 计算初始时前8张纸牌构成的peak，记录下纸牌的编号的方向
        for i in range(1, 9):
            que[i].poi = i
            if i % 2 == 1:
                que[i].dir = 1
            else:
                que[i].dir = -1
        # 峰谷首张纸牌颜色决定谁先走
        if card[1].col == 'B':
            order[9] = "Birgit"
        else:
            order[9] = "Axel"
        # 玩家交替轮流
        for i in range(10, 30):
            if order[i - 1] == "Birgit":
                order[i] = "Axel"
            else:
                order[i] = "Birgit"
        
        if Draw:
            node_count = 0
            dot = Digraph(comment='Search Graph')
            dot.node(str(node_count), "SEARCH TREE")

        # 计算name得分
        ans = dfs(9, -1, -1, -INF, 0)
        if Draw:
            dot.render('test-output/round-table' + '_NON_PRUNNING' + '.gv', view=True)
        if order[9] != name:
            ans *= -1
        if ans == 0 :
            print("Case %d: Axel and Birgit tie" % t)
        elif ans > 0:
            print("Case %d: %s wins %d" % (t, name, ans))
        else:
            print("Case %d: %s loses %d" %(t, name, -ans))

if __name__ == "__main__":
    main()

"""
Test Data
Input:
Axel
5
1R 2R 3R 4R 5R 5B 4B 3B 2B 1B 
Birgit 
5 
1R 2R 3R 4R 5R 5B 4B 3B 2B 1B 
Birgit 
5 
1R 1B 3R 4R 5R 5B 4B 3B 2R 2B 
End

Output:
Case 1: Axel wins 1 
Case 2: Birgit loses 1 
Case 3: Axel and Birgit tie 
"""