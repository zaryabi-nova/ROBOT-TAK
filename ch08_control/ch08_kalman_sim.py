import numpy as np, matplotlib; matplotlib.use('Agg'); import matplotlib.pyplot as plt
rng=np.random.default_rng(1)
dt=0.005; T=20; n=int(T/dt); t=np.arange(n)*dt
true=20*np.sin(2*np.pi*0.2*t)*(t>2)+5*(t>12)
rate=np.gradient(true,dt)
bias=0.5; gyro=rate+bias+rng.normal(0,0.3,n)
acc=true+rng.normal(0,2.0,n)
# gyro only
g=np.cumsum(gyro)*dt
# complementary
c=np.zeros(n); a=0.98
for k in range(1,n): c[k]=a*(c[k-1]+gyro[k]*dt)+(1-a)*acc[k]
# 2-state Kalman: x=[angle,bias]
Qa,Qb,R=0.001,0.003,4.0
x=np.zeros(2); P=np.eye(2); kf=np.zeros(n); bb=np.zeros(n)
for k in range(n):
    # predict
    x[0]+=dt*(gyro[k]-x[1])
    P00,P01,P10,P11=P[0,0],P[0,1],P[1,0],P[1,1]
    P[0,0]+=dt*(dt*P11-P01-P10+Qa)
    P[0,1]-=dt*P11; P[1,0]-=dt*P11; P[1,1]+=Qb*dt
    # update
    y=acc[k]-x[0]; S=P[0,0]+R; K=np.array([P[0,0]/S,P[1,0]/S])
    x+=K*y
    P00,P01=P[0,0],P[0,1]
    P[0,0]-=K[0]*P00; P[0,1]-=K[0]*P01; P[1,0]-=K[1]*P00; P[1,1]-=K[1]*P01
    kf[k]=x[0]; bb[k]=x[1]
rms=lambda e:np.sqrt(np.mean(e[n//4:]**2))
print('acc',rms(acc-true),'gyro',rms(g-true),'comp',rms(c-true),'kf',rms(kf-true),'bias est',bb[-1])
fig,ax=plt.subplots(figsize=(9,4),dpi=170)
s=slice(int(8/dt),int(16/dt))
ax.plot(t[s],acc[s],color='#bbb',lw=.6,label='accelerometer')
ax.plot(t[s],g[s],color='#e67e22',lw=1.2,label='gyro only')
ax.plot(t[s],c[s],color='#2e86c1',lw=1.2,label='complementary')
ax.plot(t[s],kf[s],color='#c0392b',lw=1.4,label='Kalman (angle+bias)')
ax.plot(t[s],true[s],'k--',lw=1,label='true angle')
ax.set_xlabel('t (s)'); ax.set_ylabel('angle (deg)'); ax.legend(fontsize=8,ncol=3,loc='lower left'); ax.grid(alpha=.3)
plt.tight_layout(); plt.savefig('kf_compare.png')
