<template>
  <div class="page">
    <div class="container">
      <div class="page-heading">
        <h1>Admin Panel</h1>
        <p>System overview and management</p>
      </div>

      <div class="stats-row">
        <div class="stat-card card" v-for="stat in stats" :key="stat.label">
          <div class="stat-icon" :style="{ background: stat.bg }" v-html="stat.icon"/>
          <div>
            <div class="stat-value">{{ stat.value }}</div>
            <div class="stat-label">{{ stat.label }}</div>
          </div>
        </div>
      </div>

      <div class="grid-2" style="margin-top:32px">
        <div class="card" style="padding:24px">
          <h2 style="font-size:1rem;margin-bottom:16px">All Users</h2>
          <div v-for="u in users" :key="u.id" class="admin-row">
            <div class="mini-av">{{ u.name.charAt(0) }}</div>
            <div style="flex:1">
              <div style="font-size:.88rem;font-weight:500">{{ u.name }}</div>
              <div style="font-size:.75rem;color:var(--c-text3)">{{ u.email }}</div>
            </div>
            <span :class="u.role === 'employer' ? 'badge badge-purple' : 'badge badge-blue'">{{ u.role }}</span>
          </div>
          <div v-if="!users.length" class="empty-state" style="padding:32px">No users yet.</div>
        </div>

        <div class="card" style="padding:24px">
          <h2 style="font-size:1rem;margin-bottom:16px">All Jobs</h2>
          <div v-for="j in jobs" :key="j.id" class="admin-row">
            <div style="flex:1">
              <div style="font-size:.88rem;font-weight:500">{{ j.title }}</div>
              <div style="font-size:.75rem;color:var(--c-text3)">{{ j.company }}</div>
            </div>
            <span :class="j.isOpen ? 'badge badge-green' : 'badge badge-red'">{{ j.isOpen ? 'Open' : 'Closed' }}</span>
          </div>
          <div v-if="!jobs.length" class="empty-state" style="padding:32px">No jobs yet.</div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import { useDataStore } from '../stores/data'

const data = useDataStore()

// Access the unwrapped value from store Refs
const users = data.users
const jobs = data.jobs
const applications = data.applications

const stats = computed(() => [
  // FIXED: Removed .value because data.users, data.jobs, etc., are already unwrapped arrays
  { label: 'Total Users', value: users.length, bg: 'rgba(79,142,247,.12)', icon: '<svg width="20" height="20" viewBox="0 0 24 24" fill="none"><path d="M17 21v-2a4 4 0 00-4-4H5a4 4 0 00-4 4v2" stroke="#4f8ef7" stroke-width="1.8"/><circle cx="9" cy="7" r="4" stroke="#4f8ef7" stroke-width="1.8"/></svg>' },
  { label: 'Total Jobs', value: jobs.length, bg: 'rgba(124,90,247,.12)', icon: '<svg width="20" height="20" viewBox="0 0 24 24" fill="none"><rect x="2" y="7" width="20" height="14" rx="2" stroke="#7c5af7" stroke-width="1.8"/><path d="M16 7V5a2 2 0 00-2-2h-4a2 2 0 00-2 2v2" stroke="#7c5af7" stroke-width="1.8"/></svg>' },
  { label: 'Applications', value: applications.length, bg: 'rgba(62,207,142,.12)', icon: '<svg width="20" height="20" viewBox="0 0 24 24" fill="none"><rect x="3" y="3" width="18" height="18" rx="2" stroke="#3ecf8e" stroke-width="1.8"/><path d="M7 8h10M7 12h10M7 16h6" stroke="#3ecf8e" stroke-width="1.8" stroke-linecap="round"/></svg>' },
])
</script>

<style scoped>
.stats-row { display: grid; grid-template-columns: repeat(auto-fill, minmax(200px,1fr)); gap: 16px; }
.stat-card { padding: 20px; display: flex; align-items: center; gap: 16px; }
.stat-icon { width: 44px; height: 44px; border-radius: 10px; display: grid; place-items: center; flex-shrink: 0; }
.stat-value { font-family: var(--font-display); font-size: 1.8rem; font-weight: 700; }
.stat-label { font-size: .8rem; color: var(--c-text2); margin-top: 4px; }
.admin-row { display: flex; align-items: center; gap: 12px; padding: 10px 0; border-bottom: 1px solid var(--c-border); }
.admin-row:last-child { border-bottom: none; }
.mini-av { width: 30px; height: 30px; border-radius: 50%; background: linear-gradient(135deg,var(--c-accent),var(--c-accent2)); display: grid; place-items: center; font-size:.75rem; font-weight:700; color:#fff; flex-shrink:0; }
</style>