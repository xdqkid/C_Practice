#include <iostream>
#include <string>
#include <string.h>
#include <ctime>
using namespace std;
bool REC_TIME = true;
bool PRUNING = false;
// bool PRUNING = true;
struct data_que
{
	int dir;
	int poi;
}que[9];

struct data_card
{
	char col;
	int dig;
}card[30];
string order[30];
int INF = 100000, n;
int calc_hold_card(int ax_hold,int bi_hold)
{
	int s = 0;
	if (ax_hold != -1)
	{
		if (card[ax_hold].col == 'R')
			s += card[ax_hold].dig;
		else 
			s -= card[ax_hold].dig;
	}
	if (bi_hold != -1)
	{
		if (card[bi_hold].col == 'B')
			s -= card[bi_hold].dig;
		else 
			s += card[bi_hold].dig;
	}
	return s;
}
void update(int &fo, int calc, int fson)
{
	if (fson != -INF)
		fo = max(fo, calc - fson);
}

int calc_digit(int pha, int pl, int p2, int p3)
{
	int counr, counb, dig;
	counr = counb = dig = 0;
	dig = card[pl].dig + card[p2].dig + card[p3].dig;
	if (card[pl].col == 'B')
		counb++;
	else
		counr++;
	if (card[p2].col == 'B')
		counb++;
	else
		counr++;
	if (card[p3].col == 'B')
		counb++;
	else
		counr++;
	if (counr > 1)
	{
		if (order[pha] == "Axel")
			return dig;
		else
			return -dig;
	}
	else
	{
		if (order[pha] == "Birgit")
			return dig;
		else
			return -dig;
	}
}

int dfs(int pha, int ax_hold, int bi_hold, int fo_calc)
{
	if (pha > 2 * n)
	{
		int s = calc_hold_card(ax_hold, bi_hold);
		if (order[pha] == "Axel")
			return s;
		else 
			return -s;
	}
	int fo, i, calc, fson;
	calc = 0;
	data_que temp_que[9];
	fo = -INF;
	memcpy(temp_que, que, sizeof(que));
	fson = INF;
	if (order[pha] == "Axel" && ax_hold == -1)
	{
		fson = dfs(pha + 1, pha, bi_hold, fo);
	}
	else if(order[pha] == "Birgit" && bi_hold == -1)
	{
		fson = dfs(pha + 1, ax_hold, pha, fo);
	}
	update(fo, calc, fson);
	if (PRUNING)
		if (fo_calc >= -fo)
			return fo;
	
	for (i = 2; i <= 6; i++)
	{
		if (que[i].dir == -1 && que[i + 1].dir == 1)
		{
			calc = calc_digit(pha, que[i].poi, que[i + 1].poi, pha);
			que[i].dir = que[i + 1].dir = 0;
			que[i].poi = pha;
			que[i + 1].poi = -1;
			fson = dfs(pha + 1, ax_hold, bi_hold, fo - calc);
			update(fo, calc ,fson);
			que[i] = temp_que[i];
			que[i + 1] = temp_que[i + 1];
			if (PRUNING)
				if (fo_calc >= -fo)
					return fo;
			if (order[pha] == "Axel" && ax_hold != -1)
			{
				calc = calc_digit(pha, que[i].poi, que[i + 1].poi, ax_hold);
				que[i].dir = que[i + 1].dir = 0;
				que[i].poi = ax_hold;
				que[i + 1].poi = -1;
				fson = dfs(pha + 1, pha, bi_hold, fo - calc);
				update(fo, calc, fson);
				que[i] = temp_que[i];
				que[i + 1] = temp_que[i + 1];
			}
			else if (order[pha] == "Birgit" && bi_hold != -1)
			{
				calc = calc_digit(pha, que[i].poi, que[i + 1].poi, bi_hold);
				que[i].dir = que[i + 1].dir = 0;
				que[i].poi = bi_hold;
				que[i + 1].poi = -1;
				fson = dfs(pha + 1, ax_hold, pha, fo - calc);
				update(fo, calc, fson);
				que[i] = temp_que[i];
				que[i + 1] = temp_que[i + 1];
			}
			que[i] = temp_que[i];
			que[i + 1] = temp_que[i + 1];
			if (PRUNING)
				if (fo_calc >= -fo)
					return fo;
		}
	}
	
	for (i = 2; i <= 6; i++)
	{
		if (que[i].dir == 0 && que[i + 1].dir == 0 && que[i].poi != -1 && que[i + 1].poi == -1)
		{
			if (order[pha] == "Axel" && ax_hold != -1)
			{
				calc = calc_digit(pha, que[i].poi, ax_hold, pha);
				que[i].dir = 1;
				que[i + 1].dir = -1;
				que[i].poi = pha;
				que[i + 1].poi = ax_hold;
				
				fson = dfs(pha + 1, -1, bi_hold, fo - calc); 
				update(fo, calc, fson);
				que[i] = temp_que[i];
				que[i + 1] = temp_que[i + 1];
				if (PRUNING)
					if (fo_calc >= -fo)
						return fo;

				calc = calc_digit(pha, que[i].poi, ax_hold, pha);
				que[i].dir = 1;
				que[i + 1].dir = -1;
				que[i].poi = ax_hold;
				que[i + 1].poi = pha;
				fson = dfs(pha + 1, -1, bi_hold, fo - calc);
				update(fo, calc, fson);
				que[i] = temp_que[i];
				que[i + 1] = temp_que[i + 1];
				if (PRUNING)
					if (fo_calc >= -fo)
						return fo;
			}
			else if (order[pha] == "Birgit" && bi_hold != -1)
			{
				calc = calc_digit(pha, que[i].poi, bi_hold, pha);
				que[i].dir = 1;
				que[i + 1].dir = -1;
				que[i].poi = pha;
				que[i + 1].poi = bi_hold;
				fson = dfs(pha + 1, ax_hold, -1, fo - calc);
				update(fo, calc, fson);
				que[i] = temp_que[i];
				que[i + 1] = temp_que[i + 1];
				if (PRUNING)
					if (fo_calc >= -fo)
						return fo;

				calc = calc_digit(pha, que[i].poi, bi_hold, pha);
				que[i].poi = bi_hold;
				que[i + 1].poi = pha;
				fson = dfs(pha + 1, ax_hold, -1, fo - calc);
				update(fo, calc, fson);
				que[i] = temp_que[i];
				que[i + 1] = temp_que[i + 1];
				if (PRUNING)
					if (fo_calc >= -fo)
						return fo;
			}
		}
	}
	return fo;
}

int main()
{
	int t = 0, i, ans, len;
	int start, end;
	string name;
	while (cin >> name && name != "End")
	{
		t++;
		cin >> n;
		for (i = 1; i <= 2 * n; i++)
		{
			char z[10];
			cin >> z;
			len = strlen(z);
			if (len == 2)
			{
				card[i].col = z[1];
				card[i].dig = z[0] - '0';
			}
			else
			{
				card[i].col = z[2];
				card[i].dig = (z[0] - '0') * 10 + (z[1] - '0');
			}
		}
		for (i = 1; i <= 8; i++)
		{
			que[i].poi = i;
			if (i % 2 == 1)
				que[i].dir = 1;
			else 
				que[i].dir = -1;
		}
		if (card[1].col == 'B')
			order[9] = "Birgit";
		else
			order[9] = "Axel";
		for (i = 10; i <= 2 * n + 2; i++)
		{
			if (order[i - 1] == "Birgit")
				order[i] = "Axel";
			else
				order[i] = "Birgit";
		}
		if (REC_TIME)
			start = clock();
		ans = dfs(9, -1, -1, -INF);
		if (REC_TIME)
			end = clock();
		if (order[9] != name)
			ans *= -1;
		if (ans == 0)
		{
			cout << "Case " << t << ": Axel and Birgit tie" << endl; 
		}
		else if(ans > 0)
		{
			cout << "Case "<< t <<": "<< name << " wins " << ans << endl;
		}
		else {
			cout << "Case " << t << ": " << name << " loses " << -ans << endl;
		}
		if (REC_TIME)
			cout << "Executing Time : " << 0.1 * (end - start) / CLOCKS_PER_SEC << endl;
	}
	return 0;
}