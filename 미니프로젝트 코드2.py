#내장 모듈
from urllib.request import *
import sys
import time

#외부 모듈
from bs4 import *
import matplotlib.pyplot as p
import matplotlib

def extractPercent () : 

    webUrl = 'https://m.entertain.naver.com/tvBrand/675566/broadcastInfo/ratingCard'
    webUrl2 = 'https://m.entertain.naver.com/tvBrand/5930336/broadcastInfo/ratingCard'

    pList = list()
    lList = list()

    wPage = urlopen(webUrl)
    soup = BeautifulSoup(wPage, 'html.parser')

    trList = soup.find_all('em', {'class': 'rating_grp_percent_num'})
    
    wPage2 = urlopen(webUrl2)
    soup2 = BeautifulSoup(wPage2, 'html.parser')

    trList = soup.find_all('em', {'class': 'rating_grp_percent_num'})

    tlList = soup2.find_all('em', {'class': 'rating_grp_percent_num'})


    for em in trList:
            pList.append(float(em.get_text()))
    for em in tlList:
        lList.append(float(em.get_text()))

    pList.reverse()
    lList.reverse()

    print("프로그램 시청률:\n")        
    print("슈퍼맨이 돌아왔다.\n",pList)
    print("미스터 션샤인\n",lList)
        
    #print (pList,"size=",len(pList))   # 결과 확인 용(확인 후 주석처리)
    return pList,lList


    
def drawGraph (pList,lList):

    
    matplotlib.rcParams['font.family']='Malgun Gothic'
    matplotlib.rcParams['axes.unicode_minus']=False
    
    x = [range(len(pList))]
    x=range(len(pList))
    y = [range(len(pList))]
    y=pList

    p.plot(x, pList,'--or')
    
    x = [range(len(lList))]
    x=range(len(lList))
    y = [range(len(lList))]
    y=lList

    p.plot(x, lList,'-ob')

    p.grid()
    p.legend(['슈퍼맨이 돌아왔다', '미스터 션샤인'],loc=4)
    p.xlabel("회차")
    p.ylabel("시청률 %")
    p.title("시청률 그래프")
    p.show()
    
 

pList,lList = extractPercent() 


drawGraph(pList,lList) # 그리기


